#pragma once

#include <windows.h>
#include <thread>
#include <mutex>
#include "sema.h"
#include <map>
#include <list>
#include <string>
#include <tuple>

// window messages to get events from mpg engine
#define WM_MPG_LIST_CHANGED (WM_APP + 1)
#define WM_MPG_STATE_CHANGED (WM_APP + 2)
#define WM_MPG_MACH4_STATUS (WM_APP + 3)

// define axis constants in Mach4 manner
#define AXIS_X 0
#define AXIS_Y 1
#define AXIS_Z 2
#define AXIS_A 3

// define Mach4 internal variable addresses
#define SV_FEEDRATE 2134

#define OSIG_MACHINE_ENABLED 1120
#define OSIG_JOG_INC 1125
#define OSIG_JOG_CONT 1126
#define OSIG_JOG_ENABLED 1127
#define OSIG_JOG_MPG 1128

#define MC_JOG_POS 1.0
#define MC_JOG_NEG -1.0

// list of events we wait from XHC device, combined for xHB03/04
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

// object class for device event
class CXhcDeviceEvent {
public:
	CXhcDeviceEvent() : m_event(nop), m_value(0) {};
	CXhcDeviceEvent(std::string name, XHC_DEV_EVENT event, int value = 0) : m_dev_name(name), m_event(event), m_value(value) { }

	// name of sending device, usually kind of USB device UID
	// setter
	void nameof(const std::string& value) { m_dev_name = value; }
	// getter
	const std::string& nameof() const { return m_dev_name; }

	// type of event
	// setter
	void eventof(XHC_DEV_EVENT value) { m_event = value; }
	// getter
	XHC_DEV_EVENT eventof() const { return m_event; }

	// additional integer data linked with this event, for example wheel speed
	// setter
	void valueof(int value) { m_value = value; }
	// getter
	int valueof() const { return m_value; }
protected:
private:
	std::string m_dev_name;
	XHC_DEV_EVENT m_event;
	int m_value;
};

// XHC wheel state
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

// XHC unit mode, we set it just for user info
enum XHC_UNITS_MODE {
	UNITS_MM,
	UNITS_INCH
};

// this class describes current state of Mach4 instance
class CM4otionState {
public:
	CM4otionState() {
		m_mc_valid = false;
		m_mc[4] = (0, 0, 0, 0);
		m_wc_valid = false;
		m_wc[4] = (0, 0, 0, 0);

		m_feedrate_ovr_valid = false;
		m_feedrate_ovr = 0;
		m_sspeed_ovr_valid = false;
		m_sspeed_ovr = 0;
		m_feedrate_valid = false;
		m_feedrate = 0;
		m_sspeed_valid = false;
		m_sspeed = 0;

		m_step_mul_valid = false;
		m_step_mul = 0;

		m_units_valid = false;
		m_units = UNITS_MM;
	}

	// machine coordinates
	// setter
	void mc(double x, double y, double z, double a) {
		m_mc_valid = true;
		m_mc[AXIS_X] = x; m_mc[AXIS_Y] = y;
		m_mc[AXIS_Z] = z; m_mc[AXIS_A] = a;
	}
	// getter by axis index
	double mc(unsigned int p) const { return m_mc[p]; }
	// flag if mc is set and valid
	bool mc_valid() const { return m_mc_valid; }

	// work coordinates
	// setter
	void wc(double x, double y, double z, double a) {
		m_wc_valid = true;
		m_wc[AXIS_X] = x; m_wc[AXIS_Y] = y;
		m_wc[AXIS_Z] = z; m_wc[AXIS_A] = a;
	}
	// getter by axis index
	double wc(unsigned int p) const { return m_wc[p]; }
	// flag if wc is set and valid
	bool wc_valid() const { return m_wc_valid; }

	// feed rate override
	// setter
	void feedrate_ovr(unsigned int v) {
		m_feedrate_ovr_valid = true;
		m_feedrate_ovr = v;
	}
	// getter
	unsigned int feedrate_ovr() const { return m_feedrate_ovr; }
	// check if valid
	bool feedrate_ovr_valid() const { return m_feedrate_ovr_valid; }

	// spindle speed override
	// setter
	void sspeed_ovr(unsigned int v) {
		m_sspeed_ovr_valid = true;
		m_sspeed_ovr = v;
	}
	// getter
	unsigned int sspeed_ovr() const { return m_sspeed_ovr; }
	// check if valid
	bool sspeed_ovr_valid() const { return m_sspeed_ovr_valid; }

	// current feed rate
	// setter
	void feedrate(unsigned int v) {
		m_feedrate_valid = true;
		m_feedrate = v;
	}
	// getter
	unsigned int feedrate() const { return m_feedrate; }
	// check if valid
	bool feedrate_valid() const { return m_feedrate_valid; }

	// current spindle speed
	// setter
	void sspeed(unsigned int v) {
		m_sspeed_valid = true;
		m_sspeed = v;
	}
	// getter
	unsigned int sspeed() const { return m_sspeed; }
	// check if valid
	bool sspeed_valid() const { return m_sspeed_valid; }

	// MPG step multiplier, currently limited to 0, 1, 5, 10, 20, 30, 40, 50, 100, 500, 1000
	// check if valid
	bool step_mul_valid() const { return m_step_mul_valid; }
	// getter
	unsigned int step_mul() const { return m_step_mul; }
	// setter, validates input parameter
	void step_mul(unsigned int v) {
		unsigned int num_steps = (sizeof(allowed_steps) / sizeof(allowed_steps[0]));
		for (unsigned int i = 0; i < num_steps; i++) {
			if (v == allowed_steps[i]) {
				m_step_mul_valid = true;
				m_step_mul = v;
				break;
			}
		}
	}
	// change multiplier to next up
	void step_mul_up() {
		unsigned int num_steps = (sizeof(allowed_steps) / sizeof(allowed_steps[0]));
		for (unsigned int i = 0; i < num_steps; i++) {
			if (m_step_mul == allowed_steps[i]) {
				m_step_mul_valid = true;
				m_step_mul = allowed_steps[(i + 1) % num_steps];
				break;
			}
		}
	}
	// change multiplier to next down
	void step_mul_down() {
		unsigned int num_steps = (sizeof(allowed_steps) / sizeof(allowed_steps[0]));
		for (unsigned int i = 0; i < num_steps; i++) {
			if (m_step_mul == allowed_steps[i]) {
				m_step_mul_valid = true;
				m_step_mul = allowed_steps[(i - 1) % num_steps];
				break;
			}
		}
	}

	// mach4 units mode - MM or INCH
	// setter
	void units(XHC_UNITS_MODE v) {
		m_units_valid = true;
		m_units = v;
	}
	// getter
	XHC_UNITS_MODE units() const { return m_units; }
	// check if valid
	bool units_valid() const { return m_units_valid; }

	// copy valid parameter from that motion state
	bool update(const CM4otionState& that) {
		bool updated = false;
		if (that.mc_valid() && (mc(AXIS_X) != that.mc(AXIS_X) || mc(AXIS_Y) != that.mc(AXIS_Y) || mc(AXIS_Z) != that.mc(AXIS_Z) || mc(AXIS_A) != that.mc(AXIS_A))) {
			mc(that.m_mc[AXIS_X], that.m_mc[AXIS_Y], that.m_mc[AXIS_Z], that.m_mc[AXIS_A]);
			updated = true;
		}
		if (that.wc_valid() && (wc(AXIS_X) != that.wc(AXIS_X) || wc(AXIS_Y) != that.wc(AXIS_Y) || wc(AXIS_Z) != that.wc(AXIS_Z) || wc(AXIS_A) != that.wc(AXIS_A))) {
			wc(that.m_wc[AXIS_X], that.m_wc[AXIS_Y], that.m_wc[AXIS_Z], that.m_wc[AXIS_A]);
			updated = true;
		}

		if (that.feedrate_ovr_valid() && feedrate_ovr() != that.feedrate_ovr()) {
			feedrate_ovr(that.feedrate_ovr());
			updated = true;
		}

		if (that.feedrate() && feedrate() != that.feedrate()) {
			feedrate(that.feedrate());
			updated = true;
		}

		if (that.sspeed_ovr_valid() && sspeed_ovr() != that.sspeed_ovr()) {
			sspeed_ovr(that.sspeed_ovr());
			updated = true;
		}

		if (that.sspeed_valid() && sspeed() != that.sspeed()) {
			sspeed(that.sspeed());
			updated = true;
		}

		if (that.units_valid() && units() != that.units()) {
			units(that.units());
			updated = true;
		}

		if (that.step_mul_valid() && step_mul() != that.step_mul()) {
			step_mul(that.step_mul());
			updated = true;
		}
		return updated;
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

	bool m_units_valid;
	XHC_UNITS_MODE m_units;

	static unsigned int allowed_steps[11];
};

// storage for XHC USB device basic properties
class CXhcDevice {
public:
	CXhcDevice() : m_type(0) {}

	// type of device, contains usb PID
	// setter
	void typeof(unsigned int value) { m_type = value; }
	// getter
	unsigned int typeof() const { return m_type; }

	// system path to usb data input interface (read data from xhc)
	// setter
	void devin(const std::string& value) { m_dev_in = value; }
	// getter
	const std::string& devin() const { return m_dev_in; }

	// system path to usb data output interface (send data to XHC)
	// setter
	void devout(const std::string& value) { m_dev_out = value; }
	// getter
	const std::string& devout() const { return m_dev_out; }

	// XHC feature report packet size, i am not sure if it can vary
	size_t feature_report_max_size() const { return 8; }
protected:
private:
	unsigned int m_type;
	std::string m_dev_in;
	std::string m_dev_out;
};

// interface definition for XHC event handler
class CXhcDeviceEventReceiver {
public:
	virtual void xhcEvent(const CXhcDeviceEvent& event) = 0;
};

// this class is doing real work with USB device
// code is working in separate thread!
class CXhcDeviceAgent {
	friend void Fagent(CXhcDeviceAgent* a) {
		a->Run();
	}
public:
	// copy all parameter for managed device and run controlling code in separate thread
	// send device events to caller provided receiver handler
	CXhcDeviceAgent(const CXhcDevice& device, CXhcDeviceEventReceiver *receiver);

	// type of USB device, contains USB PID
	unsigned int typeof() const { return m_device.typeof(); }

	// send cancelling signal to controlling thread
	void cancel() { m_cancelled = true; }
	// check if controlling thread has beed finished
	bool finished() const { return m_finished; }

	// stop and destroy controlling thread
	// this function can be called only from another thread
	void stop() {
		cancel();
		m_worker.join();
	}

	// update XHC display and state using Mach 4 motion state class
	void update(const CM4otionState& s);
protected:
	// device state
	unsigned char m_day;
	CM4otionState m_state;
	XHC_WHEEL_MODE m_wheel_mode;
	//
	CXhcDeviceEventReceiver *m_receiver;
	// managed device properties
	CXhcDevice m_device;

	// thread main loop
	virtual int Run();

	// update XHC display according to machine state
	// return false if connection is lost
	virtual bool updateDisplay(void *handle) = 0;

	// try to read and process new event from XHC device
	// return false if connection is lost
	virtual bool getEvent(void *handle, unsigned int timeout_ms) = 0;

private:
	// ugly i know but this still works
	volatile bool m_finished;
	volatile bool m_cancelled;

	semaphore m_state_sem;
	std::mutex m_state_mutex;
	std::list<CM4otionState> m_state_queue;

	// thread instance
	std::thread m_worker;
};

// old xHB03
class CXhcHB03Agent : public CXhcDeviceAgent {
public:
	CXhcHB03Agent(const CXhcDevice& device, CXhcDeviceEventReceiver *receiver) : CXhcDeviceAgent(device, receiver) {}
private:
	bool updateDisplay(void *handle) override;
	bool getEvent(void *handle, unsigned int timeout_ms) override;
};

// old xHB04
class CXhcHB04Agent : public CXhcDeviceAgent {
public:
	CXhcHB04Agent(const CXhcDevice& device, CXhcDeviceEventReceiver *receiver) : CXhcDeviceAgent(device, receiver) {}
private:
	bool updateDisplay(void *handle) override;
	bool getEvent(void *handle, unsigned int timeout_ms) override;
};

// main class which is manages list of XHC devices and controlling events flow
// it is also works in separated thread
class CXhcMpg : public CXhcDeviceEventReceiver
{
	friend void Magent(CXhcMpg* a) {
		a->run();
	}
public:

	// initialize class variables
	CXhcMpg();
	~CXhcMpg();

	// try to initialize Mach4 IPC and run main loop in separated thread
	bool open(HWND hParent);
	// destroy Mach4 IPC connection and controlling thread
	void close();

	// check list of pluged USB device to find new or removed XHC devices
	// change list of managed devices if required
	// send WINDOW(S) EVENT to hParent
	void rescan();

	// check if cancel signal was sent
	bool cancelled() const { return m_cancelled; }
	// check if controlling thread has beed finished
	bool finished() const { return m_finished; }
	// send thread cancel event
	void cancel() { m_cancelled = true; }

	// thread safe method to get list of active XHC devices
	std::list<std::wstring> devices();

	// thread safe method to get current state of Mach4 instance
	CM4otionState state() const;
protected:
	// xhc event handles (from managed usb devices)
	void xhcEvent(const CXhcDeviceEvent& event) override;

private:
	// optional WND HANDLER to send some UI notification
	HWND m_hParent;
	// Mach 4 IPC handle
	int m_ipc;

	// thread safety for list of managed xhc devices
	std::mutex m_dev_mutex;
	// list of xhc devices
	std::map<std::string, CXhcDeviceAgent*> m_devs;

	// thread safety for list of xhc events
	std::mutex m_event_mutex;
	// list of xhc events (from all pluged usb xhc devices)
	std::list<CXhcDeviceEvent> m_events;

	// yes, it is worker
	std::thread m_worker;

	// ugly but still atomic and works
	volatile bool m_cancelled;
	volatile bool m_finished;
	bool m_opened;

	// semaphore to wait events from xhc devices
	semaphore m_semaphore;

	// current state of Mach4
	CM4otionState m_state;

	// thread loop
	void run();

	// process all events available in the queue
	void handleEvent();

	// read Mach4 state using IPC
	void updateState();

	// is Mach4 machine enabled ?
	bool isMachineEnabled();

	// this vector defines jog stop timer in millisecond for each axis
	// we stop jogging for specific axis when specific timer is reaches 0
	// xhc event handler sets recessary timer when you spin the wheel
	long long m_jog_timer[4];

	// is mach4 jogging enabled ?
	bool isJogEnabled();

	// is mach4 in jog incremental mode
	bool isJogInc();
	// is mach4 in jog velocity (continuous) mode
	bool isJogCont();

	// send jog signal to MACH4 using IPC
	void jogStart(double x, double y, double z, double a);
	// send jog stop signal to MACH 4
	void jogStop(bool force, long long ms);
};
