#include "stdafx.h"
#include "CXhcMpg.h"
#include <regex>

extern "C" {
#include <hidapi.h>
}
#include "xhc_dev.h"

// i dont know why but my WHB03-L uses the same protocol as LHB04 and i don't know how to choose protocol automatically
// you can enable old protocol here
#define ENABLE_OLD_HB03 0

unsigned int CM4otionState::allowed_steps[] = { 0, 1, 5, 10, 20, 30, 40, 50, 100, 500, 1000 };

CXhcMpg::CXhcMpg() : m_semaphore{0}
{
	m_opened = false;
	m_cancelled = false;
	m_finished = false;
	m_jog_timer[AXIS_X] = m_jog_timer[AXIS_Y] = m_jog_timer[AXIS_Z] = m_jog_timer[AXIS_A] = 0;
	m_ipc = 0;
	m_hParent = 0;

	f_mcIpcInit = NULL;
	f_mcCntlGetUnitsCurrent = NULL;
	f_mcAxisGetMachinePos = NULL;
	f_mcAxisGetPos = NULL;
	f_mcAxisHomeAll = NULL;
	f_mcAxisSetPos = NULL;
	f_mcCntlCycleStart = NULL;
	f_mcCntlCycleStop = NULL;
	f_mcCntlGetFRO = NULL;
	f_mcCntlGetMode = NULL;
	f_mcCntlGetPoundVar = NULL;
	f_mcCntlGotoZero = NULL;
	f_mcCntlIsInCycle = NULL;
	f_mcCntlReset = NULL;
	f_mcCntlRewindFile = NULL;
	f_mcIpcCleanup = NULL;
	f_mcJogIncStart = NULL;
	f_mcJogIncStop = NULL;
	f_mcJogVelocityStart = NULL;
	f_mcJogVelocityStop = NULL;
	f_mcJogIsJogging = NULL;
	f_mcJogGetInc = NULL;
	f_mcScriptExecutePrivate = NULL;
	f_mcSignalGetHandle = NULL;
	f_mcSignalGetState = NULL;
	f_mcSpindleGetMotorRPM = NULL;
	f_mcSpindleGetOverride = NULL;

	m_hinstMachIpc = 0;
}

CXhcMpg::~CXhcMpg()
{
	close();
}

#define IMPORT3MACH(name) { f_##name = (decltype(##name)*) GetProcAddress(m_hinstMachIpc, #name); if (!f_##name) throw std::runtime_error("Failed to import function " #name); }

bool CXhcMpg::open(HWND hParent)
{
	if (m_opened)
		return false;

	m_hParent = hParent;
	m_cancelled = false;
	m_finished = false;

#ifdef _WIN64
	const TCHAR * knownMach4Paths[] = { _T("Mach4IPC-x64.dll"), _T("C:\\Mach4Industrial\\Mach4IPC-x64.dll"), _T("C:\\Mach4Hobby\\Mach4IPC-x64.dll") };
#else
	const TCHAR* knownMach4Paths[] = { _T("Mach4IPC.dll"), _T("C:\\Mach4Industrial\\Mach4IPC.dll"), _T("C:\\Mach4Hobby\\Mach4IPC.dll") };
#endif


	for (unsigned i = 0; i < sizeof(knownMach4Paths) / sizeof(const TCHAR *); i++) {
		m_hinstMachIpc = LoadLibrary(knownMach4Paths[i]);
		if (m_hinstMachIpc)
			break;
	}

	if (!m_hinstMachIpc) {
		throw std::exception("Failed to load Mach4IPC.dll, please put application to Mach4 folder!");
	}

	IMPORT3MACH(mcIpcInit);
	IMPORT3MACH(mcCntlGetUnitsCurrent);
	IMPORT3MACH(mcAxisGetMachinePos);
	IMPORT3MACH(mcAxisGetPos);
	IMPORT3MACH(mcAxisHomeAll);
	IMPORT3MACH(mcAxisSetPos);
	IMPORT3MACH(mcCntlCycleStart);
	IMPORT3MACH(mcCntlCycleStop);
	IMPORT3MACH(mcCntlGetFRO);
	IMPORT3MACH(mcCntlGetMode);
	IMPORT3MACH(mcCntlGetPoundVar);
	IMPORT3MACH(mcCntlGotoZero);
	IMPORT3MACH(mcCntlIsInCycle);
	IMPORT3MACH(mcCntlReset);
	IMPORT3MACH(mcCntlRewindFile);
	IMPORT3MACH(mcIpcCleanup);
	IMPORT3MACH(mcJogIsJogging);
	IMPORT3MACH(mcJogIncStart);
	IMPORT3MACH(mcJogIncStop);
	IMPORT3MACH(mcJogVelocityStart);
	IMPORT3MACH(mcJogVelocityStop);
	IMPORT3MACH(mcJogGetInc);
	IMPORT3MACH(mcScriptExecutePrivate);
	IMPORT3MACH(mcSignalGetHandle);
	IMPORT3MACH(mcSignalGetState);
	IMPORT3MACH(mcSpindleGetMotorRPM);
	IMPORT3MACH(mcSpindleGetOverride);

	// scan list of pluged usb devices to find XHC pendants
	rescan();

	// try to connect to Mach 4 instance using mach4ipc interface
	if (f_mcIpcInit("localhost") != 0)
		return false;

	// it seems mach 4 ipc handle is always = 0
	m_ipc = 0;

	// get mach 4 units mode
	int units = 0;
	if (f_mcCntlGetUnitsCurrent(m_ipc, &units) == 0) {
		if (units == 200) {
			m_state.units(UNITS_INCH);
		}
		else {
			m_state.units(UNITS_MM);
		}
	}

	// start main code in separated thread
	m_worker = std::thread([this]() {
			run();
		});
	m_opened = true;

	// return to caller
	return true;
}

void CXhcMpg::close()
{
	if (!m_opened)
		return;

	// send cancel event to thread
	cancel();
	// wait thread to terminate
	m_worker.join();

	// cancel and destroy all threads for managed XHC devices
	for (auto const &[path, t] : m_devs) {
		if (t) {
			t->stop();
		}
	}
	m_devs.clear();

	// destroy connection to Mach 4 IPC
	f_mcIpcCleanup();
	FreeLibrary(m_hinstMachIpc);
	m_hinstMachIpc = NULL;
	m_opened = false;
}

bool CXhcMpg::isMachineEnabled()
{
	HMCSIG h;
	if (f_mcSignalGetHandle(m_ipc, OSIG_MACHINE_ENABLED, &h) == 0) {
		BOOL state;
		if (f_mcSignalGetState(h, &state) == 0) {
			return state == TRUE;
		}
	}
	throw std::exception("Mach4IPC connection error");
}

bool CXhcMpg::isJogEnabled()
{
	HMCSIG h;
	if (f_mcSignalGetHandle(m_ipc, OSIG_JOG_ENABLED, &h) == 0) {
		BOOL state;
		if (f_mcSignalGetState(h, &state) == 0) {
			return state == TRUE;
		}
	}
	throw std::exception("Mach4IPC connection error");
}

bool CXhcMpg::isJogCont()
{
	HMCSIG h;
	if (f_mcSignalGetHandle(m_ipc, OSIG_JOG_CONT, &h) == 0) {
		BOOL state;
		if (f_mcSignalGetState(h, &state) == 0) {
			return state == TRUE;
		}
	}
	throw std::exception("Mach4IPC connection error");
}

bool CXhcMpg::isJogInc()
{
	HMCSIG h;

	int rc = f_mcSignalGetHandle(m_ipc, OSIG_JOG_INC, &h);
	if (rc == 0) {
		BOOL state;
		if (f_mcSignalGetState(h, &state) == 0) {
			return state == TRUE;
		}
	}
	throw std::exception("Mach4IPC connection error");
}

void CXhcMpg::jogMove(double x, double y, double z, double a)
{
	long long time_to_stop = 100;
	bool inc_jog = isJogInc();

	//	inc_jog = false;

	if (inc_jog) {
		// TODO:
		// mcJogSetInc(m_ipc, X_AXIS, ????)
		// mcJogSetAccel ???
		// mcJogGetInc ??

		// mcMotionGetIncPos
		// mcJogIsJogging
		// mcJogGetInc

		auto _inc_move = [this, _increment = 0.0, _is_jogging = FALSE](int axisid, double v) mutable
		{
			if (v == 0.0)
				return 0.0;

			if (f_mcJogGetInc(m_ipc, axisid, &_increment) != MERROR_NOERROR)
				return 0.0;

			_increment *= v > 0 ? 1.0 : -1.0;
#if 0
			if (f_mcJogIsJogging(m_ipc, axisid, &_is_jogging) != MERROR_NOERROR)
				return 0.0;

			if (_is_jogging) {
				double _val;
				if (f_mcMotionGetIncPos(m_ipc, axisid, &_val) != MERROR_NOERROR)
					return 0.0;
			}
			else {
				f_mcJogIncStart(m_ipc, axisid, _increment);
			}
#endif
			f_mcJogIncStart(m_ipc, axisid, _increment);

			return _increment;
		};

		double inc_vector[4] = { 0.0, 0.0, 0.0, 0.0 };
		if (x != 0) {
			inc_vector[0] = _inc_move(AXIS_X, x);
			m_jog_timer[AXIS_X] = time_to_stop;
		}
		if (y != 0) {
			f_mcJogIncStart(m_ipc, AXIS_Y, y);
			m_jog_timer[AXIS_Y] = time_to_stop;
		}
		if (z != 0) {
			f_mcJogIncStart(m_ipc, AXIS_Z, z);
			m_jog_timer[AXIS_Z] = time_to_stop;
		}
		if (a != 0) {
			f_mcJogIncStart(m_ipc, AXIS_A, a);
			m_jog_timer[AXIS_A] = time_to_stop;
		}
	}
	else {
		// TODO:
		// mcJogSetRate(m_ipc, X_AXIS, ????)
		// mcJogSetAccel ???
		if (x != 0) {
			f_mcJogVelocityStart(m_ipc, AXIS_X, x > 0.0 ? MC_JOG_POS : MC_JOG_NEG);
			m_jog_timer[AXIS_X] = time_to_stop;
		}
		if (y != 0) {
			f_mcJogVelocityStart(m_ipc, AXIS_Y, y > 0.0 ? MC_JOG_POS : MC_JOG_NEG);
			m_jog_timer[AXIS_Y] = time_to_stop;
		}
		if (z != 0) {
			f_mcJogVelocityStart(m_ipc, AXIS_Z, z > 0.0 ? MC_JOG_POS : MC_JOG_NEG);
			m_jog_timer[AXIS_Z] = time_to_stop;
		}
		if (a != 0) {
			f_mcJogVelocityStart(m_ipc, AXIS_A, a > 0.0 ? MC_JOG_POS : MC_JOG_NEG);
			m_jog_timer[AXIS_A] = time_to_stop;
		}
	}
}

void CXhcMpg::jogStop(bool force, long long ms)
{
	try {
		unsigned int axis[4] = { AXIS_X, AXIS_Y, AXIS_Z, AXIS_A };

		for (unsigned int i = 0; i < 4; i++) {
			unsigned int a = axis[i];
			if (m_jog_timer[a]) {
				if (!force && m_jog_timer[a] > ms) {
					m_jog_timer[a] -= ms;
				}
				else {
					bool is_inc = isJogInc();
					if (is_inc) {
						double _increment;
						if (f_mcJogGetInc(m_ipc, a, &_increment) != MERROR_NOERROR)
							_increment = 0.1;
						f_mcJogIncStop(m_ipc, a, _increment); // no ideas what does mean incr there
					}
					else {
						f_mcJogVelocityStop(m_ipc, a);
					}
					m_jog_timer[a] = 0;
				}
			}
		}
	}
	catch ([[maybe_unused]] const std::exception& e) {

	}
}

void CXhcMpg::handleEvent()
{
	try {
		while (1) {
			std::unique_ptr<CXhcDeviceEvent> event;
			{
				std::lock_guard<std::mutex> lock(m_event_mutex);

				if (m_events.size() == 0)
					break;

				event = std::move(m_events.front());
				m_events.pop_front();
			}
			switch (event->eventof()) {
			case btnStop:
				f_mcCntlCycleStop(m_ipc);
				break;
			case btnStartPause:
			{
				BOOL cycle;
				if (f_mcCntlIsInCycle(m_ipc, &cycle) == 0) {
					if (!cycle)
						f_mcCntlCycleStart(m_ipc);
					else
						f_mcCntlCycleStop(m_ipc);
				}
			}
			break;
			case btnReset:
				f_mcCntlReset(m_ipc);
				break;
			case btnRewind:
				f_mcCntlRewindFile(m_ipc);
				break;
			case btnGotoZero:
				f_mcCntlGotoZero(m_ipc);
				break;
			case btnGotoHome:
				f_mcAxisHomeAll(m_ipc);
				break;
			case btnZeroX:
				f_mcAxisSetPos(m_ipc, AXIS_X, 0.0);
				break;
			case btnZeroY:
				f_mcAxisSetPos(m_ipc, AXIS_Y, 0.0);
				break;
			case btnZeroZ:
				f_mcAxisSetPos(m_ipc, AXIS_Z, 0.0);
				break;
			case btnProbeZ:
				f_mcScriptExecutePrivate(m_ipc, "XHC\\xhc.probez.lua", FALSE);
				break;
			case adjustX:
				jogMove(event->valueof(), 0, 0, 0);
				break;
			case adjustY:
				jogMove(0, event->valueof(), 0, 0);
				break;
			case adjustZ:
				jogMove(0, 0, event->valueof(), 0);
				break;
			case adjustA:
				jogMove(0, 0, 0, event->valueof());
				break;
			case btnStepPlusPlus:
			case btnStepRight:
				m_state.step_mul_up();
				break;
			case btnStepLeft:
				m_state.step_mul_down();
				break;
			}
		}
	}
	catch ([[maybe_unused]] const std::exception& e) {

	}
}

CM4otionState CXhcMpg::state() const
{
	// todo: lock access to m_state
	return m_state;
}

bool CXhcMpg::updateState()
{
	CM4otionState s;
	double x, y, z, a;
	int rc, mode;

	rc = f_mcCntlGetMode(m_ipc, &mode);
	if (rc) {
		// check specific code ?
		return false;
	}

	rc = f_mcAxisGetPos(m_ipc, AXIS_X, &x);
	rc = f_mcAxisGetPos(m_ipc, AXIS_Y, &y);
	rc = f_mcAxisGetPos(m_ipc, AXIS_Z, &z);
	rc = f_mcAxisGetPos(m_ipc, AXIS_A, &a);
	s.wc(x, y, z, a);

	rc = f_mcAxisGetMachinePos(m_ipc, AXIS_X, &x);
	rc = f_mcAxisGetMachinePos(m_ipc, AXIS_Y, &y);
	rc = f_mcAxisGetMachinePos(m_ipc, AXIS_Z, &z);
	rc = f_mcAxisGetMachinePos(m_ipc, AXIS_A, &a);
	s.mc(x, y, z, a);

	f_mcSpindleGetOverride(m_ipc, &x);
	s.sspeed_ovr((unsigned int)abs((int)x));

	f_mcSpindleGetMotorRPM(m_ipc, &x);
	s.sspeed((unsigned int)abs((int)x));

	f_mcCntlGetFRO(m_ipc, &x);
	s.feedrate_ovr((unsigned int)abs((int)x));

	f_mcCntlGetPoundVar(m_ipc, SV_FEEDRATE, &x);
	s.feedrate((unsigned int)abs((int)x));

	if (m_state.update(s)) {
		if (m_hParent) {
			PostMessage(m_hParent, WM_MPG_STATE_CHANGED, 0, 0);
		}
	}
	std::lock_guard<std::mutex> lock(m_dev_mutex);
	for (auto const&[guid, dev] : m_devs) {
		dev->update(m_state);
	}
	return true;
}

void CXhcMpg::run()
{
	std::chrono::time_point<std::chrono::system_clock> updateTime = std::chrono::system_clock::now() + std::chrono::milliseconds(500);
	std::chrono::time_point<std::chrono::system_clock> jogTimerBegin = std::chrono::system_clock::now();

	if (m_hParent) {
		PostMessage(m_hParent, WM_MPG_MACH4_STATUS, 1, 0);
	}

	while (!cancelled()) {
		// wait for key/wheel events from plugged XHC devices
		if (m_semaphore.try_acquire_for(std::chrono::milliseconds(100))) {
			handleEvent();
		}
		// refresh devices displays every X milliseconds
		if (std::chrono::system_clock::now() > updateTime) {
			updateTime = std::chrono::system_clock::now() + std::chrono::milliseconds(500);
			if (!updateState()) {
				// it seems we lost connection to MACH
				cancel();
			}
		}
		if (!cancelled()) {
			std::chrono::time_point<std::chrono::system_clock> jogTimerEnd = std::chrono::system_clock::now();

			long long jog_delta = std::chrono::duration_cast<std::chrono::milliseconds>(jogTimerEnd - jogTimerBegin).count();
			if (jog_delta) {
				jogStop(false, jog_delta);
			}

			jogTimerBegin = std::chrono::system_clock::now();
		}
	}
	jogStop(true, 0);
	m_finished = true;

	if (m_hParent) {
		PostMessage(m_hParent, WM_MPG_MACH4_STATUS, 0, 0);
	}
}

void CXhcMpg::xhcEvent(std::unique_ptr<CXhcDeviceEvent> pevent)
{
	std::lock_guard<std::mutex> lock(m_event_mutex);
	m_events.emplace_back(std::move(pevent));
	m_semaphore.release();
}

void CXhcMpg::rescan()
{
	std::regex re("^(.+)\\&Col\\d{2}(#.+)\\&\\d+(#\\{.*)$", std::regex::ECMAScript | std::regex::optimize);
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
				std::string guid = match[1].str() + match[2].str() + match[3].str();

				const auto& dev1 = cur_xhc_devs.find(guid);
				if (dev1 == cur_xhc_devs.end()) {
					CXhcDevice device;

					device.typeof(cur_dev->product_id);
					if (path.find("&Col01") != std::string::npos) {
						device.devin(path);
					}
					else if (path.find("&Col02") != std::string::npos) {
						device.devout(path);
					}
					cur_xhc_devs.emplace(guid, device);
				}
				else {
					if (path.find("&Col01") != std::string::npos) {
						dev1->second.devin(path);
					}
					else if (path.find("&Col02") != std::string::npos) {
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
				std::string guid = match[1].str() + match[2].str() + match[3].str();

				const auto& dev1 = cur_xhc_devs.find(guid);
				if (dev1 == cur_xhc_devs.end()) {
					CXhcDevice device;

					device.typeof(cur_dev->product_id);
					if (path.find("&Col01") != std::string::npos) {
						device.devin(path);
					}
					else if (path.find("&Col02") != std::string::npos) {
						device.devout(path);
					}
					cur_xhc_devs.emplace(guid, device);
				}
				else {
					if (path.find("&Col01") != std::string::npos) {
						dev1->second.devin(path);
					}
					else if (path.find("&Col02") != std::string::npos) {
						dev1->second.devout(path);
					}
				}
			}
		}
		cur_dev = cur_dev->next;
	}
	hid_free_enumeration(devs);

	bool list_changed = false;

	std::lock_guard<std::mutex> lock(m_dev_mutex);
	for (auto const&[guid, dev] : cur_xhc_devs) {
		const auto& d = m_devs.find(guid);
		if (d == m_devs.end()) {
			switch (dev.typeof()) {
			case WHB03_PID:
				m_devs.emplace(guid, std::make_unique<CXhcHB03Agent>(dev, this));
				list_changed = true;
				break;
			case WHB04_PID:
				m_devs.emplace(guid, std::make_unique<CXhcHB04Agent>(dev, this));
				list_changed = true;
				break;
			}
		}
		else {
			if (d->second->finished()) {
				d->second->stop();
				m_devs.erase(d);
				switch (dev.typeof()) {
				case WHB03_PID:
					m_devs.emplace(guid, new CXhcHB03Agent(dev, this));
					list_changed = true;
					break;
				case WHB04_PID:
					m_devs.emplace(guid, new CXhcHB04Agent(dev, this));
					list_changed = true;
					break;
				}
			}
		}
	}

	for (auto dit = m_devs.begin(); dit != m_devs.end(); ) {
		if (cur_xhc_devs.find(dit->first) == cur_xhc_devs.end()) {
			dit->second->stop();
			dit = m_devs.erase(dit);
			list_changed = true;
		}
		else
			dit++;
	}

	if (list_changed) {
		if (m_hParent) {
			PostMessage(m_hParent, WM_MPG_LIST_CHANGED, 0, 0);
		}
	}
}

std::list<std::wstring> CXhcMpg::devices()
{
	std::list<std::wstring> list;

	std::lock_guard<std::mutex> lock(m_dev_mutex);
	for (auto const&[guid, dev] : m_devs) {
		if (!dev->finished()) {
			std::wstring node_name;

			switch (dev->typeof()) {
			case WHB03_PID:
				node_name = _T("XHC xHB03 MPG");
				break;
			case WHB04_PID:
				node_name = _T("XHC xHB04 MPG");
				break;
			case WHB04B6_PID:
				node_name = _T("XHC WHB04B-6 MPG");
				break;
			default:
				node_name = _T("UNKNOWN MPG");
			}
			list.push_back(node_name);
		}
	}

	return list;
}

CXhcDeviceAgent::CXhcDeviceAgent(const CXhcDevice& device, CXhcDeviceEventReceiver* receiver) : m_device(device), m_receiver(receiver),
m_state_sem{ 0 }
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
	m_wheel_axis = WHEEL_OFF;
	m_worker = std::thread([this]() {
		Run();
	});
}

#define _defract(c) { (uint16_t) abs(c), (uint8_t) (((uint8_t) abs((c - (long)c)*100.0)) | (c < 0 ? 0x80 : 0))}
#define _defract2(c) { (uint16_t) abs(c), (uint16_t) (((uint16_t) abs((c - (long)c)*10000.0)) | (c < 0 ? 0x8000 : 0)) }

void CXhcDeviceAgent::update(const CM4otionState& s)
{
	std::lock_guard<std::mutex> lock(m_state_mutex);
	m_state_queue.push_back(s);

	m_state_sem.release();
}

int CXhcDeviceAgent::Run()
{
	hid_device *hin = hid_open_path(m_device.devin().c_str());
	hid_device *hout = hid_open_path(m_device.devout().c_str());

	if (hin && hout) {
		while (!m_cancelled) {
			if (!getEvent(hin, 50))
				break;

			if (m_state_sem.try_acquire()) {
				std::lock_guard<std::mutex> lock(m_state_mutex);

				for (auto const& state : m_state_queue) {
					m_state.update(state);
				}
				m_state_queue.clear();
				updateDisplay(hout);
			}
		}

		hid_close(hin);
		hid_close(hout);
	}

	m_finished = true;

	return 0;
}

#define _defract(c) { (uint16_t) abs(c), (uint8_t) (((uint8_t) abs((c - (long)c)*100.0)) | (c < 0 ? 0x80 : 0))}
#define _defract2(c) { (uint16_t) abs(c), (uint16_t) (((uint16_t) abs((c - (long)c)*10000.0)) | (c < 0 ? 0x8000 : 0)) }

bool CXhcHB03Agent::updateDisplay(void *handle)
{
#if ENABLE_OLD_HB03
	whb03_out_data cmd = {
		WHBxx_MAGIC,
		m_day,
		{
			_defract(((m_wheel_axis == WHEEL_A) ? m_state.wc(AXIS_A) : m_state.wc(AXIS_X))),
			_defract(m_state.wc(AXIS_Y)),
			_defract(m_state.wc(AXIS_Z)),
			_defract(((m_wheel_axis == WHEEL_A) ? m_state.mc(AXIS_A) : m_state.mc(AXIS_X))),
			_defract(m_state.mc(AXIS_Y)),
			_defract(m_state.mc(AXIS_Z))
		},
		(uint16_t)m_state.feedrate_ovr(),
		(uint16_t)m_state.sspeed_ovr(),
		(uint16_t)m_state.feedrate(),
		(uint16_t)m_state.sspeed(),

		0,
		0
	};
#else
	whb04_out_data cmd = {
		WHBxx_MAGIC,
		m_day,
		{
			_defract2(((m_wheel_axis == WHEEL_A) ? m_state.wc(AXIS_A) : m_state.wc(AXIS_X))),
			_defract2(m_state.wc(AXIS_Y)),
			_defract2(m_state.wc(AXIS_Z)),
			_defract2(((m_wheel_axis == WHEEL_A) ? m_state.mc(AXIS_A) : m_state.mc(AXIS_X))),
			_defract2(m_state.mc(AXIS_Y)),
			_defract2(m_state.mc(AXIS_Z))
		},
		(uint16_t)m_state.feedrate_ovr(),
		(uint16_t)m_state.sspeed_ovr(),
		(uint16_t)m_state.feedrate(),
		(uint16_t)m_state.sspeed(),
		0,
		0
	};
#endif

	switch (m_state.step_mul()) {
	case 0:
		cmd.step_mul = 0;
		break;
	case 1:
		cmd.step_mul = 1;
		break;
	case 5:
		cmd.step_mul = 2;
		break;
	case 10:
		cmd.step_mul = 3;
		break;
	case 20:
		cmd.step_mul = 4;
		break;
	case 30:
		cmd.step_mul = 5;
		break;
	case 40:
		cmd.step_mul = 6;
		break;
	case 50:
		cmd.step_mul = 7;
		break;
	case 100:
		cmd.step_mul = 8;
		break;
	case 500:
		cmd.step_mul = 9;
		break;
	case 1000:
		cmd.step_mul = 10;
		break;
	}

	if (m_state.units() == UNITS_INCH)
		cmd.state |= WHB03_STATE_UNIT_INCH;

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

	return true;
}

bool CXhcHB03Agent::getEvent(void *handle, unsigned int timeout_ms)
{
	unsigned char packet[64];
	int packet_len = sizeof(whb0x_in_data);

	int rc = hid_read_timeout((hid_device *)handle, packet, packet_len, timeout_ms);
	if (rc < 0)
		return false;

	if (rc == packet_len) {
		whb0x_in_data *pkt = (whb0x_in_data *)packet;
		if (pkt->id == 4 && pkt->xor_day == (m_day ^ pkt->btn_1)) {
			std::unique_ptr<CXhcDeviceEvent> pevent = std::make_unique<CXhcDeviceEvent>();

			pevent->nameof(m_device.devin());

			switch (pkt->axis) {
			case 0x11:
				m_wheel_axis = WHEEL_X;
				break;
			case 0x12:
				m_wheel_axis = WHEEL_Y;
				break;
			case 0x13:
				m_wheel_axis = WHEEL_Z;
				break;
			case 0x14:
				m_wheel_axis = WHEEL_FEED;
				break;
			case 0x15:
				m_wheel_axis = WHEEL_SPINDLE;
				break;
			case 0x18:
				m_wheel_axis = WHEEL_A;
				break;
			default:
				m_wheel_axis = WHEEL_OFF;
			}

			switch (pkt->btn_1) {
			case 0x17:
				pevent->eventof(btnYes);
				break;
			case 0x16:
				pevent->eventof(btnNo);
				break;
			case 0x01:
				pevent->eventof(btnStop);
				break;
			case 0x02:
				pevent->eventof(btnStartPause);
				break;
			case 0x03:
				pevent->eventof(btnRewind);
				break;
			case 0x04:
				pevent->eventof(btnProbeZ);
				break;
			case 0x0c:
				pevent->eventof(btnGotoZero);
				break;
			case 0x06:
				pevent->eventof(btnXDiv2);
				break;
			case 0x07:
				pevent->eventof(btnYDiv2);
				break;
			case 0x08:
				pevent->eventof(btnSafeZ);
				break;
			case 0x09:
				pevent->eventof(btnZeroX);
				break;
			case 0x0a:
				pevent->eventof(btnZeroY);
				break;
			case 0x0b:
				pevent->eventof(btnZeroZ);
				break;
			case 0x05:
				pevent->eventof(btnGotoHome);
				break;
			case 0x0d:
				pevent->eventof(btnStepLeft);
				break;
			case 0x0e:
				pevent->eventof(btnStepRight);
				break;
			case 0x0f:
				pevent->eventof(btnFineXYZ);
				break;
			case 0x10:
				pevent->eventof(btnSpindle);
				break;
			}
			// send button event if happened
			if (pevent->eventof()) {
				if (m_receiver) {
					m_receiver->xhcEvent(std::move(pevent));
				}
				pevent = std::make_unique<CXhcDeviceEvent>();

				pevent->nameof(m_device.devin());
			}

			if (pkt->jog_counts) {
				pevent->valueof(pkt->jog_counts);
				switch (pkt->axis) {
				case 0x11:
					pevent->eventof(adjustX);
					break;
				case 0x12:
					pevent->eventof(adjustY);
					break;
				case 0x13:
					pevent->eventof(adjustZ);
					break;
				case 0x14:
					pevent->eventof(adjustFeedRate);
					break;
				case 0x15:
					pevent->eventof(adjustSpindleSpeed);
					break;
				case 0x18:
					pevent->eventof(adjustProcessingSpeed);
					break;
				}
				// send wheel event if happened
				if (pevent->eventof()) {
					if (m_receiver) {
						m_receiver->xhcEvent(std::move(pevent));
					}
				}
			}
		}
	}
	return true;
}

bool CXhcHB04Agent::updateDisplay(void *handle)
{
	whb04_out_data cmd = {
		WHBxx_MAGIC,
		m_day,
		{
			_defract2(((m_wheel_axis == WHEEL_A) ? m_state.wc(AXIS_A) : m_state.wc(AXIS_X))),
			_defract2(m_state.wc(AXIS_Y)),
			_defract2(m_state.wc(AXIS_Z)),
			_defract2(((m_wheel_axis == WHEEL_A) ? m_state.mc(AXIS_A) : m_state.mc(AXIS_X))),
			_defract2(m_state.mc(AXIS_Y)),
			_defract2(m_state.mc(AXIS_Z))
		},
		(uint16_t)m_state.feedrate_ovr(),
		(uint16_t)m_state.sspeed_ovr(),
		(uint16_t)m_state.feedrate(),
		(uint16_t)m_state.sspeed(),
		0,
		0
	};

	switch (m_state.step_mul()) {
	case 0:
		cmd.step_mul = 0;
		break;
	case 1:
		cmd.step_mul = 1;
		break;
	case 5:
		cmd.step_mul = 2;
		break;
	case 10:
		cmd.step_mul = 3;
		break;
	case 20:
		cmd.step_mul = 4;
		break;
	case 30:
		cmd.step_mul = 5;
		break;
	case 40:
		cmd.step_mul = 6;
		break;
	case 50:
		cmd.step_mul = 7;
		break;
	case 100:
		cmd.step_mul = 8;
		break;
	case 500:
		cmd.step_mul = 9;
		break;
	case 1000:
		cmd.step_mul = 10;
		break;
	}

	if (m_state.units() == UNITS_INCH)
		cmd.state |= WHB04_STATE_UNIT_INCH;

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

	return true;
}

bool CXhcHB04Agent::getEvent(void *handle, unsigned int timeout_ms)
{
	unsigned char packet[64];
	int packet_len = sizeof(whb0x_in_data);

	int rc = hid_read_timeout((hid_device *)handle, packet, packet_len, timeout_ms);
	if (rc < 0)
		return false;

	if (rc == packet_len) {
		whb0x_in_data *pkt = (whb0x_in_data *)packet;
		if (pkt->id == 4 && pkt->xor_day == (m_day ^ pkt->btn_1)) {
			std::unique_ptr<CXhcDeviceEvent> pevent = std::make_unique<CXhcDeviceEvent>();

			pevent->nameof(m_device.devin());

			switch (pkt->axis) {
			case 0x11:
				m_wheel_axis = WHEEL_X;
				break;
			case 0x12:
				m_wheel_axis = WHEEL_Y;
				break;
			case 0x13:
				m_wheel_axis = WHEEL_Z;
				break;
			case 0x14:
				m_wheel_axis = WHEEL_FEED;
				break;
			case 0x15:
				m_wheel_axis = WHEEL_SPINDLE;
				break;
			case 0x18:
				m_wheel_axis = WHEEL_A;
				break;
			default:
				m_wheel_axis = WHEEL_OFF;
			}

			switch (pkt->btn_1) {
			case 0x17:
				pevent->eventof(btnReset);
				break;
			case 0x16:
				pevent->eventof(btnStop);
				break;
			case 0x01:
				pevent->eventof(btnGotoZero);
				break;
			case 0x02:
				pevent->eventof(btnStartPause);
				break;
			case 0x03:
				pevent->eventof(btnRewind);
				break;
			case 0x04:
				pevent->eventof(btnProbeZ);
				break;
			case 0x0c:
				pevent->eventof(btnSpindle);
				break;
			case 0x06:
				switch (pkt->axis) {
				case 0x11:
					pevent->eventof(btnXDiv2);
					break;
				case 0x12:
					pevent->eventof(btnYDiv2);
					break;
				case 0x13:
					pevent->eventof(btnZDiv2);
					break;
				}
				break;
			case 0x07:
				switch (pkt->axis) {
				case 0x11:
					pevent->eventof(btnZeroX);
					break;
				case 0x12:
					pevent->eventof(btnZeroY);
					break;
				case 0x13:
					pevent->eventof(btnZeroZ);
					break;
				}
				break;
			case 0x08:
				pevent->eventof(btnSafeZ);
				break;
			case 0x09:
				pevent->eventof(btnGotoHome);
				break;
			case 0x0a:
				pevent->eventof(btnMacro1);
				break;
			case 0x0b:
				pevent->eventof(btnMacro2);
				break;
			case 0x05:
				pevent->eventof(btnMacro3);
				break;
			case 0x0d:
				pevent->eventof(btnStepPlusPlus);
				break;
			case 0x0e:
				pevent->eventof(btnMpgMode);
				break;
			case 0x0f:
				pevent->eventof(btnMacro6);
				break;
			case 0x10:
				pevent->eventof(btnMacro7);
				break;
			}
			// send button event if happened
			if (pevent->eventof()) {
				if (m_receiver) {
					m_receiver->xhcEvent(std::move(pevent));
				}
				pevent->eventof(nop);
			}

			if (pkt->jog_counts) {
				pevent->valueof(pkt->jog_counts);
				switch (pkt->axis) {
				case 0x11:
					pevent->eventof(adjustX);
					break;
				case 0x12:
					pevent->eventof(adjustY);
					break;
				case 0x13:
					pevent->eventof(adjustZ);
					break;
				case 0x14:
					pevent->eventof(adjustFeedRate);
					break;
				case 0x15:
					pevent->eventof(adjustSpindleSpeed);
					break;
				case 0x18:
					pevent->eventof(adjustA);
					break;
				}
				// send wheel event if happened
				if (pevent->eventof()) {
					if (m_receiver) {
						m_receiver->xhcEvent(std::move(pevent));
					}
				}
			}
		}
	}
	return true;
}
