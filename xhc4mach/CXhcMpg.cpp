#include "stdafx.h"
#include "CXhcMpg.h"
#include "Mach4IPC.h"
#include <regex>

extern "C" {
#include <hidapi.h>
}
#include "xhc_dev.h"

CXhcMpg::CXhcMpg()
{
	m_opened = false;
	m_cancelled = false;
	m_ipc = 0;
}

CXhcMpg::~CXhcMpg()
{
	close();
}

bool CXhcMpg::open()
{
	if (m_opened)
		return false;

	if (mcIpcInit("localhost") != 0)
		return false;

	m_ipc = 0;

	rescan();

	m_worker = std::thread(Magent, this);
	m_opened = true;

	return true;
}

void CXhcMpg::close()
{
	if (!m_opened)
		return;

	cancel();
	m_worker.join();

	for (auto const &[path, t] : m_devs) {
		if (t) {
			t->stop();
			delete t;
		}
	}
	mcIpcCleanup();
	m_opened = false;
}

void CXhcMpg::handleEvent()
{
	std::lock_guard<std::mutex> lock(m_event_mutex);

	while (m_events.begin() != m_events.end()) {
		auto event = m_events.begin();
		switch (event->eventof()) {
		case adjustX:
			//			mcJogIncStart(0, 0, event->valueof());
			break;
		}
		m_events.pop_front();
	}
}

void CXhcMpg::updateState()
{
	CM4otionState s;
	double x, y, z, a;
	int rc;

	rc = mcAxisGetPos(m_ipc, AXIS_X, &x);
	rc = mcAxisGetPos(m_ipc, AXIS_Y, &y);
	rc = mcAxisGetPos(m_ipc, AXIS_Z, &z);
	rc = mcAxisGetPos(m_ipc, AXIS_A, &a);
	s.wc(x, y, z, a);

	rc = mcAxisGetMachinePos(m_ipc, AXIS_X, &x);
	rc = mcAxisGetMachinePos(m_ipc, AXIS_Y, &y);
	rc = mcAxisGetMachinePos(m_ipc, AXIS_Z, &z);
	rc = mcAxisGetMachinePos(m_ipc, AXIS_A, &a);
	s.mc(x, y, z, a);

	mcSpindleGetOverride(m_ipc, &x);
	s.sspeed_ovr((unsigned int)abs((int)x));

	mcSpindleGetMotorRPM(m_ipc, &x);
	s.sspeed((unsigned int) abs((int)x));

	mcCntlGetFRO(m_ipc, &x);
	s.feedrate_ovr((unsigned int)abs((int)x));

	mcCntlGetPoundVar(m_ipc, SV_FEEDRATE, &x);
	s.feedrate((unsigned int)abs((int)x));

	std::lock_guard<std::mutex> lock(m_dev_mutex);
	for (auto const&[guid, dev] : m_devs) {
		dev->update(s);
	}
}

void CXhcMpg::run()
{
	std::chrono::time_point<std::chrono::system_clock> updateTime = std::chrono::system_clock::now() + std::chrono::milliseconds(500);

	while (!cancelled()) {
		if (m_semaphore.wait_for(std::chrono::milliseconds(100))) {
			handleEvent();
		}
		if (std::chrono::system_clock::now() > updateTime) {
			updateTime = std::chrono::system_clock::now() + std::chrono::milliseconds(500);
			updateState();
		}
	}
}

void CXhcMpg::xhcEvent(const CXhcDeviceEvent& event)
{
	std::lock_guard<std::mutex> lock(m_event_mutex);
	m_events.push_back(event);
	m_semaphore.notify();
}

void CXhcMpg::rescan()
{
	std::regex re("#\\{([^\\}]+)\\}$", std::regex::ECMAScript | std::regex::optimize);
	std::map<std::string, CXhcDevice> cur_xhc_devs;
	struct hid_device_info *devs, *cur_dev;

	devs = hid_enumerate(0, 0);
	cur_dev = devs;
	while (cur_dev) {
		if (cur_dev->vendor_id == WHBxx_VID && cur_dev->product_id == WHB03_PID && cur_dev->path) {
			std::string path(cur_dev->path);
			// extract device guid
			std::smatch match;
			if (std::regex_search(path, match, re) && match.size()) {
				std::string guid = match[1];

				auto& dev1 = cur_xhc_devs.find(guid);
				if (dev1 == cur_xhc_devs.end()) {
					CXhcDevice device;

					device.typeof(cur_dev->product_id);
					if (path.find("&col01") != std::string::npos) {
						device.devin(path);
					}
					else if (path.find("&col02") != std::string::npos) {
						device.devout(path);
					}
					cur_xhc_devs.emplace(guid, device);
				}
				else {
					if (path.find("&col01") != std::string::npos) {
						dev1->second.devin(path);
					}
					else if (path.find("&col02") != std::string::npos) {
						dev1->second.devout(path);
					}
				}
			}
		}
		else if (cur_dev->vendor_id == WHBxx_VID && cur_dev->product_id == WHB04_PID && cur_dev->path) {
			std::string path(cur_dev->path);
			// extract device guid
			std::smatch match;
			if (std::regex_search(path, match, re) && match.size()) {
				std::string guid = match[1];

				auto& dev1 = cur_xhc_devs.find(guid);
				if (dev1 == cur_xhc_devs.end()) {
					CXhcDevice device;

					device.typeof(cur_dev->product_id);
					if (path.find("&col01") != std::string::npos) {
						device.devin(path);
					}
					else if (path.find("&col02") != std::string::npos) {
						device.devout(path);
					}
					cur_xhc_devs.emplace(guid, device);
				}
				else {
					if (path.find("&col01") != std::string::npos) {
						dev1->second.devin(path);
					}
					else if (path.find("&col02") != std::string::npos) {
						dev1->second.devout(path);
					}
				}
			}
		}
		cur_dev = cur_dev->next;
	}
	hid_free_enumeration(devs);

	std::lock_guard<std::mutex> lock(m_dev_mutex);
	for (auto const&[guid, dev] : cur_xhc_devs) {
		const auto& d = m_devs.find(guid);
		if (d == m_devs.end()) {
			m_devs.emplace(guid, new CXhcDeviceAgent(dev, this));
		}
		else if (d->second->finished()) {
			d->second->stop();
			delete d->second;
			m_devs.erase(d);
			m_devs.emplace(guid, new CXhcDeviceAgent(dev, this));
		}
	}
}

CXhcDeviceAgent::CXhcDeviceAgent(const CXhcDevice& device, CXhcDeviceEventReceiver *receiver) : m_device(device), m_receiver(receiver)
{
	m_finished = false;
	m_cancelled = false;

	unsigned char day = (255 * rand()) / RAND_MAX;
	time_t now = time(0);
	struct tm t;
	if (localtime_s(&t, &now) == 0) {
		day = t.tm_mday;
	}
	m_day = day;
	m_wheel_mode = WHEEL_OFF;
	m_worker = std::thread(Fagent, this);
}

#define _defract(c) { (uint16_t) abs(c), (uint8_t) (((uint8_t) ((c - (long)c)*100.0)) | (c < 0 ? 0x80 : 0))}
#define _defract2(c) { (uint16_t) abs(c), (uint16_t) (((uint16_t) ((c - (long)c)*10000.0)) | (c < 0 ? 0x8000 : 0)) }

void CXhcDeviceAgent::update(const CM4otionState& s)
{
	std::lock_guard<std::mutex> lock(m_state_mutex);
	m_state_queue.push_back(s);

	m_state_sem.notify();
}

bool CXhcDeviceAgent::send_status(void *handle)
{
	switch (typeof()) {
	case WHB03_PID:
	{
		whb03_out_data cmd = {
			WHBxx_MAGIC,
			m_day,
			{
				_defract(((m_wheel_mode == WHEEL_A) ? m_state.wc(AXIS_A) : m_state.wc(AXIS_X))),
				_defract(m_state.wc(AXIS_Y)),
				_defract(m_state.wc(AXIS_Z)),
				_defract(((m_wheel_mode == WHEEL_A) ? m_state.mc(AXIS_A) : m_state.mc(AXIS_X))),
				_defract(m_state.mc(AXIS_Y)),
				_defract(m_state.mc(AXIS_Z))
			},
			(uint16_t)m_state.feedrate_ovr(),
			(uint16_t)m_state.sspeed_ovr(),
			(uint16_t)m_state.feedrate(),
			(uint16_t)m_state.sspeed(),

			(uint8_t)m_state.step_mul(),
			(uint8_t)m_state.state()
		};
		size_t cmd_len = sizeof(cmd), max_pkt_len = 8;
		uint8_t *p = (uint8_t *)&cmd, pkt[128];
		pkt[0] = 0x06;
		while (cmd_len) {
			size_t len = cmd_len > (max_pkt_len - 1) ? (max_pkt_len - 1) : cmd_len;
			memcpy(&pkt[1], p, len);
			int rc = hid_send_feature_report((hid_device *)handle, pkt, max_pkt_len);
			if (rc != max_pkt_len) {
				return false;
			}
			p += len;
			cmd_len -= len;
		}
	}
	break;
	case WHB04_PID:
	{
		whb04_out_data cmd = {
			WHBxx_MAGIC,
			m_day,
			{
				_defract2(((m_wheel_mode == WHEEL_A) ? m_state.wc(AXIS_A) : m_state.wc(AXIS_X))),
				_defract2(m_state.wc(AXIS_Y)),
				_defract2(m_state.wc(AXIS_Z)),
				_defract2(((m_wheel_mode == WHEEL_A) ? m_state.mc(AXIS_A) : m_state.mc(AXIS_X))),
				_defract2(m_state.mc(AXIS_Y)),
				_defract2(m_state.mc(AXIS_Z))
			},
			(uint16_t)m_state.feedrate_ovr(),
			(uint16_t)m_state.sspeed_ovr(),
			(uint16_t)m_state.feedrate(),
			(uint16_t)m_state.sspeed(),

			(uint8_t)m_state.step_mul(),
			(uint8_t)m_state.state()
		};
		size_t cmd_len = sizeof(cmd), max_pkt_len = 8;
		uint8_t *p = (uint8_t *)&cmd, pkt[128];
		pkt[0] = 0x06;
		while (cmd_len) {
			size_t len = cmd_len > (max_pkt_len - 1) ? (max_pkt_len - 1) : cmd_len;
			memcpy(&pkt[1], p, len);
			int rc = hid_send_feature_report((hid_device *)handle, pkt, max_pkt_len);
			if (rc != max_pkt_len) {
				return false;
			}
			p += len;
			cmd_len -= len;
		}
	}
	break;
	default:
		return false;
	}

	return true;
}

int CXhcDeviceAgent::Run()
{
	hid_device *hin = hid_open_path(m_device.devin().c_str());
	hid_device *hout = hid_open_path(m_device.devout().c_str());

	if (hin && hout) {
		unsigned char packet[64];
		int packet_len;

		switch (typeof()) {
		case WHB03_PID:
		case WHB04_PID:
			packet_len = sizeof(whb0x_in_data);
			break;
		default:
			m_cancelled = true;
		}

		while (!m_cancelled) {
			int rc = hid_read_timeout(hin, packet, packet_len, 100);
			if (rc < 0)
				break;
			if (rc == packet_len) {
				switch (typeof()) {
				case WHB03_PID:
				{
					whb0x_in_data *pkt = (whb0x_in_data *)packet;
					if (pkt->id == 4 && pkt->xor_day == (m_day ^ pkt->btn_1)) {
						CXhcDeviceEvent event;

						event.nameof(m_device.devin());

						switch (pkt->wheel_mode) {
						case 0x11:
							m_wheel_mode = WHEEL_X;
							break;
						case 0x12:
							m_wheel_mode = WHEEL_Y;
							break;
						case 0x13:
							m_wheel_mode = WHEEL_Z;
							break;
						case 0x14:
							m_wheel_mode = WHEEL_FEED;
							break;
						case 0x15:
							m_wheel_mode = WHEEL_SPINDLE;
							break;
						case 0x18:
							m_wheel_mode = WHEEL_A;
							break;
						default:
							m_wheel_mode = WHEEL_OFF;
						}

						switch (pkt->btn_1) {
						case 0x17:
							event.eventof(btnYes);
							break;
						case 0x16:
							event.eventof(btnNo);
							break;
						case 0x01:
							event.eventof(btnStop);
							break;
						case 0x02:
							event.eventof(btnStartPause);
							break;
						case 0x03:
							event.eventof(btnRewind);
							break;
						case 0x04:
							event.eventof(btnProbeZ);
							break;
						case 0x0c:
							event.eventof(btnGotoZero);
							break;
						case 0x06:
							event.eventof(btnXDiv2);
							break;
						case 0x07:
							event.eventof(btnYDiv2);
							break;
						case 0x08:
							event.eventof(btnSafeZ);
							break;
						case 0x09:
							event.eventof(btnZeroX);
							break;
						case 0x0a:
							event.eventof(btnZeroY);
							break;
						case 0x0b:
							event.eventof(btnZeroZ);
							break;
						case 0x05:
							event.eventof(btnGotoHome);
							break;
						case 0x0d:
							event.eventof(btnStepLeft);
							break;
						case 0x0e:
							event.eventof(btnStepRight);
							break;
						case 0x0f:
							event.eventof(btnFineXYZ);
							break;
						case 0x10:
							event.eventof(btnSpindle);
							break;
						}
						// send button event if happened
						if (event.eventof()) {
							if (m_receiver) {
								m_receiver->xhcEvent(event);
							}
							event.eventof(nop);
						}

						if (pkt->wheel) {
							event.valueof(pkt->wheel);
							switch (pkt->wheel_mode) {
							case 0x11:
								event.eventof(adjustX);
								break;
							case 0x12:
								event.eventof(adjustY);
								break;
							case 0x13:
								event.eventof(adjustZ);
								break;
							case 0x14:
								event.eventof(adjustFeedRate);
								break;
							case 0x15:
								event.eventof(adjustSpindleSpeed);
								break;
							case 0x18:
								event.eventof(adjustProcessingSpeed);
								break;
							}
							// send wheel event if happened
							if (event.eventof()) {
								if (m_receiver) {
									m_receiver->xhcEvent(event);
								}
							}
						}
					}
				}
				break;
				case WHB04_PID:
				{
					whb0x_in_data *pkt = (whb0x_in_data *)packet;
					if (pkt->id == 4 && pkt->xor_day == (m_day ^ pkt->btn_1)) {
						CXhcDeviceEvent event;

						event.nameof(m_device.devin());

						switch (pkt->wheel_mode) {
						case 0x11:
							m_wheel_mode = WHEEL_X;
							break;
						case 0x12:
							m_wheel_mode = WHEEL_Y;
							break;
						case 0x13:
							m_wheel_mode = WHEEL_Z;
							break;
						case 0x14:
							m_wheel_mode = WHEEL_FEED;
							break;
						case 0x15:
							m_wheel_mode = WHEEL_SPINDLE;
							break;
						case 0x18:
							m_wheel_mode = WHEEL_A;
							break;
						default:
							m_wheel_mode = WHEEL_OFF;
						}

						switch (pkt->btn_1) {
						case 0x17:
							event.eventof(btnReset);
							break;
						case 0x16:
							event.eventof(btnStop);
							break;
						case 0x01:
							event.eventof(btnGotoZero);
							break;
						case 0x02:
							event.eventof(btnStartPause);
							break;
						case 0x03:
							event.eventof(btnRewind);
							break;
						case 0x04:
							event.eventof(btnProbeZ);
							break;
						case 0x0c:
							event.eventof(btnSpindle);
							break;
						case 0x06:
							switch (pkt->wheel_mode) {
							case 0x11:
								event.eventof(btnXDiv2);
								break;
							case 0x12:
								event.eventof(btnYDiv2);
								break;
							case 0x13:
								event.eventof(btnZDiv2);
								break;
							}
							break;
						case 0x07:
							switch (pkt->wheel_mode) {
							case 0x11:
								event.eventof(btnZeroX);
								break;
							case 0x12:
								event.eventof(btnZeroY);
								break;
							case 0x13:
								event.eventof(btnZeroZ);
								break;
							}
							break;
						case 0x08:
							event.eventof(btnSafeZ);
							break;
						case 0x09:
							event.eventof(btnGotoHome);
							break;
						case 0x0a:
							event.eventof(btnMacro1);
							break;
						case 0x0b:
							event.eventof(btnMacro2);
							break;
						case 0x05:
							event.eventof(btnMacro3);
							break;
						case 0x0d:
							event.eventof(btnStepPlusPlus);
							break;
						case 0x0e:
							event.eventof(btnMpgMode);
							break;
						case 0x0f:
							event.eventof(btnMacro6);
							break;
						case 0x10:
							event.eventof(btnMacro7);
							break;
						}
						// send button event if happened
						if (event.eventof()) {
							if (m_receiver) {
								m_receiver->xhcEvent(event);
							}
							event.eventof(nop);
						}

						if (pkt->wheel) {
							event.valueof(pkt->wheel);
							switch (pkt->wheel_mode) {
							case 0x11:
								event.eventof(adjustX);
								break;
							case 0x12:
								event.eventof(adjustY);
								break;
							case 0x13:
								event.eventof(adjustZ);
								break;
							case 0x14:
								event.eventof(adjustFeedRate);
								break;
							case 0x15:
								event.eventof(adjustSpindleSpeed);
								break;
							case 0x18:
								event.eventof(adjustA);
								break;
							}
							// send wheel event if happened
							if (event.eventof()) {
								if (m_receiver) {
									m_receiver->xhcEvent(event);
								}
							}
						}
					}
				}
				break;
				}
			}
			if (m_state_sem.try_wait()) {
				std::lock_guard<std::mutex> lock(m_state_mutex);

				for (auto const& state : m_state_queue) {
					m_state.update(state);
				}
				m_state_queue.clear();
				send_status(hout);
			}
		}

		hid_close(hin);
		hid_close(hout);
	}

	m_finished = true;

	return 0;
}
