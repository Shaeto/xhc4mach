#pragma once
#include <thread>
#include <mutex>
#include "sema.h"
#include <map>
#include <list>
#include <string>
#include <tuple>

#define AXIS_X 0
#define AXIS_Y 1
#define AXIS_Z 2
#define AXIS_A 3

#define SV_FEEDRATE 2134

enum XHC_DEV_EVENT {
	nop,
	btnYes,
	btnNo,
	btnReset,
	btnStop,
	btnStartPause,
	btnRewind,
	btnProbeZ,
	btnGotoZero,
	btnXDiv2,
	btnYDiv2,
	btnZDiv2,
	btnSafeZ,
	btnZeroX,
	btnZeroY,
	btnZeroZ,
	btnGotoHome,
	btnStepLeft,
	btnStepRight,
	btnFineXYZ,
	btnSpindle,
	btnMacro1,
	btnMacro2,
	btnMacro3,
	btnMacro4,
	btnMacro5,
	btnMacro6,
	btnMacro7,
	btnStepPlusPlus,
	btnMpgMode,
	adjustFeedRate,
	adjustSpindleSpeed,
	adjustProcessingSpeed,
	adjustX,
	adjustY,
	adjustZ,
	adjustA
};

class CXhcDeviceEvent {
public:
	CXhcDeviceEvent() : m_event(nop), m_value(0) {};
	CXhcDeviceEvent(std::string name, XHC_DEV_EVENT event, int value = 0) : m_dev_name(name), m_event(event), m_value(value) { }

	void nameof(const std::string& value) { m_dev_name = value; }
	const std::string& nameof() const { return m_dev_name; }

	void eventof(XHC_DEV_EVENT value) { m_event = value; }
	XHC_DEV_EVENT eventof() const { return m_event; }

	void valueof(int value) { m_value = value; }
	int valueof() const { return m_value; }
protected:
private:
	std::string m_dev_name;
	XHC_DEV_EVENT m_event;
	int m_value;
};

class CM4otionState {
public:
	CM4otionState() {
		m_mc_valid = false;
		m_mc[4] = ( 0, 0, 0, 0 );
		m_wc_valid = false;
		m_wc[4] = ( 0, 0, 0, 0 );

		m_feedrate_ovr_valid = false;
		m_feedrate_ovr = 0;
		m_sspeed_ovr_valid = false;
		m_sspeed_ovr = 0;
		m_feedrate_valid = false;
		m_feedrate = 0;
		m_sspeed_valid = false;
		m_sspeed = 0;
	}

	void mc(double x, double y, double z, double a) {
		m_mc_valid = true;
		m_mc[AXIS_X] = x; m_mc[AXIS_Y] = y;
		m_mc[AXIS_Z] = z; m_mc[AXIS_A] = a;
	}

	void wc(double x, double y, double z, double a) {
		m_wc_valid = true;
		m_wc[AXIS_X] = x; m_wc[AXIS_Y] = y;
		m_wc[AXIS_Z] = z; m_wc[AXIS_A] = a;
	}

	bool mc_valid() const { return m_mc_valid; }
	double mc(unsigned int p) const { return m_mc[p]; }

	bool wc_valid() const { return m_wc_valid; }
	double wc(unsigned int p) const { return m_wc[p]; }

	bool feedrate_ovr_valid() const { return m_feedrate_ovr_valid; }
	unsigned int feedrate_ovr() const { return m_feedrate_ovr; }
	void feedrate_ovr(unsigned int v) {
		m_feedrate_ovr_valid = true;
		m_feedrate_ovr = v;
	}

	bool sspeed_ovr_valid() const { return m_sspeed_ovr_valid; }
	unsigned int sspeed_ovr() const { return m_sspeed_ovr; }
	void sspeed_ovr(unsigned int v) {
		m_sspeed_ovr_valid = true;
		m_sspeed_ovr = v;
	}

	bool feedrate_valid() const { return m_feedrate_valid; }
	unsigned int feedrate() const { return m_feedrate; }
	void feedrate(unsigned int v) {
		m_feedrate_valid = true;
		m_feedrate = v;
	}

	bool sspeed_valid() const { return m_sspeed_valid; }
	unsigned int sspeed() const { return m_sspeed; }
	void sspeed(unsigned int v) {
		m_sspeed_valid = true;
		m_sspeed = v;
	}

	bool step_mul_valid() const { return m_step_mul_valid; }
	unsigned int step_mul() const { return m_step_mul; }
	void step_mul(unsigned int v) {
		m_step_mul_valid = true;
		m_step_mul = v;
	}

	bool state_valid() const { return m_state_valid; }
	unsigned int state() const { return m_state; }
	void state(unsigned int v) {
		m_state_valid = true;
		m_state = v;
	}

	void update(const CM4otionState& that) {
		if (that.mc_valid()) {
			mc(that.m_mc[AXIS_X], that.m_mc[AXIS_Y], that.m_mc[AXIS_Z], that.m_mc[AXIS_A]);
		}
		if (that.wc_valid()) {
			wc(that.m_wc[AXIS_X], that.m_wc[AXIS_Y], that.m_wc[AXIS_Z], that.m_wc[AXIS_A]);
		}

		if (that.feedrate_ovr_valid())
			feedrate_ovr(that.feedrate_ovr());
		if (that.feedrate())
			feedrate(that.feedrate());

		if (that.sspeed_ovr_valid())
			sspeed_ovr(that.sspeed_ovr());
		if (that.sspeed_valid())
			sspeed(that.sspeed());

		if (that.state_valid())
			state(that.state());

		if (that.step_mul_valid())
			step_mul(that.step_mul());
	}

protected:
private:
	bool m_mc_valid;
	double m_mc[4];

	bool m_wc_valid;
	double m_wc[4];

	bool m_feedrate_ovr_valid;
	unsigned int m_feedrate_ovr;

	bool m_sspeed_ovr_valid;
	unsigned int m_sspeed_ovr;

	bool m_feedrate_valid;
	unsigned int m_feedrate;

	bool m_sspeed_valid;
	unsigned int m_sspeed;

	bool m_step_mul_valid;
	unsigned int m_step_mul;

	bool m_state_valid;
	unsigned int m_state;
};

enum XHC_WHEEL_MODE {
	WHEEL_OFF,
	WHEEL_X,
	WHEEL_Y,
	WHEEL_Z,
	WHEEL_A,
	WHEEL_SPINDLE,
	WHEEL_FEED,
	WHEEL_PROC
};

class CXhcDevice {
public:
	CXhcDevice() : m_type(0) {}

	void typeof(unsigned int value) { m_type = value; }
	unsigned int typeof() const { return m_type; }

	void devin(const std::string& value) { m_dev_in = value; }
	const std::string& devin() const { return m_dev_in; }

	void devout(const std::string& value) { m_dev_out = value; }
	const std::string& devout() const { return m_dev_out; }
	size_t feature_report_max_size() const { return 8; }
protected:
private:
	unsigned int m_type;
	std::string m_dev_in;
	std::string m_dev_out;
};

class CXhcDeviceEventReceiver {
public:
	virtual void xhcEvent(const CXhcDeviceEvent& event) = 0;
};

class CXhcDeviceAgent {
	friend void Fagent(CXhcDeviceAgent* a) {
		a->Run();
	}
public:
	CXhcDeviceAgent(const CXhcDevice& device, CXhcDeviceEventReceiver *receiver);

	unsigned int typeof() const { return m_device.typeof(); }

	void cancel() { m_cancelled = true; }
	bool finished() const { return m_finished; }

	void stop() {
		cancel();
		m_worker.join();
	}

	void update(const CM4otionState& s);
protected:
	virtual int Run();

private:
	volatile bool m_finished;
	volatile bool m_cancelled;

	CXhcDevice m_device;

	// device state
	unsigned char m_day;
	CM4otionState m_state;

	semaphore m_state_sem;
	std::mutex m_state_mutex;
	std::list<CM4otionState> m_state_queue;

	XHC_WHEEL_MODE m_wheel_mode;

	//
	CXhcDeviceEventReceiver *m_receiver;

	bool send_status(void *handle);

	std::thread m_worker;
};

class CXhcMpg : public CXhcDeviceEventReceiver
{
	friend void Magent(CXhcMpg* a) {
		a->run();
	}
public:
	CXhcMpg();
	~CXhcMpg();

	bool open();
	void close();

	void rescan();

	bool cancelled() const { return m_cancelled; }
	void cancel() { m_cancelled = true; }
protected:
	void xhcEvent(const CXhcDeviceEvent& event) override;

private:
	int m_ipc;

	std::mutex m_dev_mutex;
	std::map<std::string, CXhcDeviceAgent*> m_devs;

	std::mutex m_event_mutex;
	std::list<CXhcDeviceEvent> m_events;

	std::thread m_worker;

	volatile bool m_cancelled;
	bool m_opened;
	semaphore m_semaphore;

	void run();
	void handleEvent();
	void updateState();
};
