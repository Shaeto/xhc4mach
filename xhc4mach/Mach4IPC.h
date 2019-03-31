#pragma once

#include <stdint.h>

/* imported from Mach4IPC.dll and Mach4CoreAPI.chm */

#ifdef MAKE_M4DEF
#define M4IAPI __declspec(dllexport)
#define DUMMY_BODY_RINT {return 0;}
#else
#define M4IAPI __declspec(dllimport)
#define DUMMY_BODY_RINT
#endif
#define MACH_ENTRY __stdcall
#define MC_MAX_AXIS_MOTORS 4
#define MAX_MAPPED_SIGNAL 32

// handlers
typedef uint32_t MINSTANCE;
typedef uint32_t HMINSTANCE;
typedef uint32_t HMCVARS;
typedef uint32_t HMCDEV;
typedef uint32_t HMCSIG;
typedef uint32_t HMCIO;
typedef uint32_t HMCREG;

typedef uint32_t mcState;

#ifdef __cplusplus
extern "C" {
#endif

	// ---- mcAxisDeref ----
	M4IAPI int MACH_ENTRY mcAxisDeref(
		MINSTANCE mInst,
		int axisId) DUMMY_BODY_RINT;


	// ---- mcAxisDerefAll ----
	M4IAPI int MACH_ENTRY mcAxisDerefAll(
		MINSTANCE mInst) DUMMY_BODY_RINT;


	// ---- mcAxisEnable ----
	M4IAPI int MACH_ENTRY mcAxisEnable(
		MINSTANCE mInst,
		int axisId,
		BOOL enabled) DUMMY_BODY_RINT;


	// ---- mcAxisGetAuxPos ----
	/* mcAxisGetAuxPos */

	// ---- mcAxisGetHomeDir ----
	M4IAPI int MACH_ENTRY mcAxisGetHomeDir(
		MINSTANCE mInst,
		int axisId,
		int *dir) DUMMY_BODY_RINT;


	// ---- mcAxisGetHomeInPlace ----
	M4IAPI int MACH_ENTRY mcAxisGetHomeInPlace(
		MINSTANCE mInst,
		int axisId,
		BOOL *homeInPlace) DUMMY_BODY_RINT;


	// ---- mcAxisGetHomeOffset ----
	M4IAPI int MACH_ENTRY mcAxisGetHomeOffset(
		MINSTANCE mInst,
		int axisId,
		double *offset) DUMMY_BODY_RINT;


	// ---- mcAxisGetHomeOrder ----
	M4IAPI int MACH_ENTRY mcAxisGetHomeOrder(
		MINSTANCE mInst,
		int axisId,
		int *order) DUMMY_BODY_RINT;


	// ---- mcAxisGetHomeSpeed ----
	M4IAPI int MACH_ENTRY mcAxisGetHomeSpeed(
		MINSTANCE mInst,
		int axisId,
		double *percent) DUMMY_BODY_RINT;


	// ---- mcAxisGetInfoStruct ----

	struct axisinfo {
		BOOL OutOfBandAxis;     // Is this an out of band axis?
		BOOL IsStill;           // Set high when the axis is not moving
		BOOL  Jogging;          // Used to tell to jog...
		BOOL  Homing;           // Used to tell the state of the home operation.
		int  Id;                // Axis Id
		BOOL IsSpindle;         // Does this axis control a spindle?
		BOOL Enabled;           // Is axis enabled?
		BOOL SoftlimitEnabled;  // Softlimits enabled?
		double SoftMaxLimit;    // Count for the max travel.
		double SoftMinLimit;    // Count for the m.
		BOOL VelocityMode;      // Used to make the axis move at a fixed speed
		BOOL BufferJog;         // Buffer all jogs?
		double Pos;             // Position in user units.
		double Mpos;            // Machine position in user units.
		int HomeOrder;          // The order in which to home the axis.
		int HomeDir;            // The direction the axis homes.
		double HomeOffset;      // The offset from the the limits switch.
		double HomeSpeedPercent;// The percentage of the max velocity at which to home.
		BOOL SoftlimitUsed;     // Use Softlimits?
		BOOL HomeInPlace;       // Zero the axis in place when Refed?
		int MotorId[MC_MAX_AXIS_MOTORS]; //child motor ID array.
	};
	typedef struct axisinfo axisinfo_t;
	M4IAPI int MACH_ENTRY mcAxisGetInfoStruct(
		MINSTANCE mInst,
		int axisId,
		axisinfo_t *ainf) DUMMY_BODY_RINT;


	// ---- mcAxisGetMachineAuxPos ----
	/* mcAxisGetMachineAuxPos */

	// ---- mcAxisGetMachinePos ----
	M4IAPI int MACH_ENTRY mcAxisGetMachinePos(
		MINSTANCE mInst,
		int axisId,
		double *val) DUMMY_BODY_RINT;


	// ---- mcAxisGetMotorId ----
	M4IAPI int MACH_ENTRY mcAxisGetMotorId(
		MINSTANCE mInst,
		int axis,
		int childId,
		int *motorId) DUMMY_BODY_RINT;


	// ---- mcAxisGetOverrideAxis ----
	M4IAPI int MACH_ENTRY mcAxisGetOverrideAxis(
		MINSTANCE mInst,
		int axis,
		int *axis1,
		int *axis2,
		int *axis3,
		int *axis4) DUMMY_BODY_RINT;


	// ---- mcAxisGetPos ----
	M4IAPI int MACH_ENTRY mcAxisGetPos(
		MINSTANCE mInst,
		int axisId,
		double *val) DUMMY_BODY_RINT;


	// ---- mcAxisGetProbePos ----
	M4IAPI int MACH_ENTRY mcAxisGetProbePos(
		MINSTANCE mInst,
		int axisId,
		BOOL machinePos,
		double *val) DUMMY_BODY_RINT;


	// ---- mcAxisGetProbePosAll ----
	M4IAPI int MACH_ENTRY mcAxisGetProbePosAll(
		MINSTANCE mInst,
		BOOL machinePos,
		double *x,
		double *y,
		double *z,
		double *a,
		double *b,
		double *c) DUMMY_BODY_RINT;


	// ---- mcAxisGetScale ----
	M4IAPI int MACH_ENTRY mcAxisGetScale(
		MINSTANCE mInst,
		int axisId,
		double *scaleVal) DUMMY_BODY_RINT;


	// ---- mcAxisGetSoftlimitEnable ----
	M4IAPI int MACH_ENTRY mcAxisGetSoftlimitEnable(
		MINSTANCE mInst,
		int axisId,
		int *enable) DUMMY_BODY_RINT;


	// ---- mcAxisGetSoftlimitMax ----
	M4IAPI int MACH_ENTRY mcAxisGetSoftlimitMax(
		MINSTANCE mInst,
		int axisId,
		double *max) DUMMY_BODY_RINT;


	// ---- mcAxisGetSoftlimitMin ----
	M4IAPI int MACH_ENTRY mcAxisGetSoftlimitMin(
		MINSTANCE mInst,
		int axisId,
		double *min) DUMMY_BODY_RINT;


	// ---- mcAxisGetSpindle ----
	M4IAPI int MACH_ENTRY mcAxisGetSpindle(
		MINSTANCE mInst,
		int axisId,
		bool *spindle) DUMMY_BODY_RINT;


	// ---- mcAxisGetVel ----
	M4IAPI int MACH_ENTRY mcAxisGetVel(
		MINSTANCE mInst,
		int axisId,
		double *velocity) DUMMY_BODY_RINT;


	// ---- mcAxisHome ----
	M4IAPI int MACH_ENTRY mcAxisHome(
		MINSTANCE mInst,
		int axisId) DUMMY_BODY_RINT;


	// ---- mcAxisHomeAll ----
	M4IAPI int MACH_ENTRY mcAxisHomeAll(
		MINSTANCE mInst) DUMMY_BODY_RINT;


	// ---- mcAxisHomeComplete ----
	M4IAPI int MACH_ENTRY mcAxisHomeComplete(
		MINSTANCE mInst,
		int axisId) DUMMY_BODY_RINT;


	// ---- mcAxisHomeCompleteWithStatus ----
	M4IAPI int MACH_ENTRY mcAxisHomeCompleteWithStatus(
		MINSTANCE mInst,
		int axisId,
		BOOL success) DUMMY_BODY_RINT;


	// ---- mcAxisIsEnabled ----
	M4IAPI int MACH_ENTRY mcAxisIsEnabled(
		MINSTANCE mInst,
		int axisId,
		BOOL *enabled) DUMMY_BODY_RINT;


	// ---- mcAxisIsHomed ----
	M4IAPI int MACH_ENTRY mcAxisIsHomed(
		MINSTANCE mInst,
		int axisId,
		BOOL *homed) DUMMY_BODY_RINT;


	// ---- mcAxisIsHoming ----
	M4IAPI int MACH_ENTRY mcAxisIsHoming(
		MINSTANCE mInst,
		int axisId,
		BOOL *homing) DUMMY_BODY_RINT;


	// ---- mcAxisIsStill ----
	M4IAPI int MACH_ENTRY mcAxisIsStill(
		MINSTANCE mInst,
		int axisId,
		BOOL *still) DUMMY_BODY_RINT;


	// ---- mcAxisMapMotor ----
	M4IAPI int MACH_ENTRY mcAxisMapMotor(
		MINSTANCE mInst,
		int axisId,
		int motorId) DUMMY_BODY_RINT;


	// ---- mcAxisRegister ----
	M4IAPI int MACH_ENTRY mcAxisRegister(
		MINSTANCE mInst,
		int axisId) DUMMY_BODY_RINT;


	// ---- mcAxisRemoveOverrideAxis ----
	M4IAPI int MACH_ENTRY mcAxisRemoveOverrideAxis(
		MINSTANCE mInst,
		int axisId,
		int axisToOverrideId) DUMMY_BODY_RINT;


	// ---- mcAxisRemoveOverrideAxisSync ----
	M4IAPI int MACH_ENTRY mcAxisRemoveOverrideAxisSync(
		MINSTANCE mInst,
		int axisId,
		int axisToOverrideId) DUMMY_BODY_RINT;


	// ---- mcAxisSetHomeDir ----
	M4IAPI int MACH_ENTRY mcAxisSetHomeDir(
		MINSTANCE mInst,
		int axisId,
		int dir) DUMMY_BODY_RINT;


	// ---- mcAxisSetHomeInPlace ----
	M4IAPI int MACH_ENTRY mcAxisSetHomeInPlace(
		MINSTANCE mInst,
		int axisId,
		BOOL homeInPlace) DUMMY_BODY_RINT;


	// ---- mcAxisSetHomeOffset ----
	M4IAPI int MACH_ENTRY mcAxisSetHomeOffset(
		MINSTANCE mInst,
		int axisId,
		double offset) DUMMY_BODY_RINT;


	// ---- mcAxisSetHomeOrder ----
	M4IAPI int MACH_ENTRY mcAxisSetHomeOrder(
		MINSTANCE mInst,
		int axisId,
		int order) DUMMY_BODY_RINT;


	// ---- mcAxisSetHomeSpeed ----
	M4IAPI int MACH_ENTRY mcAxisSetHomeSpeed(
		MINSTANCE mInst,
		int axis,
		double percent) DUMMY_BODY_RINT;


	// ---- mcAxisSetMachinePos ----
	M4IAPI int MACH_ENTRY mcAxisSetMachinePos(
		MINSTANCE mInst,
		int axis,
		double val) DUMMY_BODY_RINT;


	// ---- mcAxisSetOverrideAxis ----
	M4IAPI int MACH_ENTRY mcAxisSetOverrideAxis(
		MINSTANCE mInst,
		int axisTd,
		int axisToOverrideId) DUMMY_BODY_RINT;


	// ---- mcAxisSetPos ----
	M4IAPI int MACH_ENTRY mcAxisSetPos(
		MINSTANCE mInst,
		int axisId,
		double val) DUMMY_BODY_RINT;


	// ---- mcAxisSetScale ----
	/* mcAxisSetScale */

	// ---- mcAxisSetSoftlimitEnable ----
	M4IAPI int MACH_ENTRY mcAxisSetSoftlimitEnable(
		MINSTANCE mInst,
		int axis,
		int enabel) DUMMY_BODY_RINT;


	// ---- mcAxisSetSoftlimitMax ----
	M4IAPI int MACH_ENTRY mcAxisSetSoftlimitMax(
		MINSTANCE mInst,
		int axisId,
		double max) DUMMY_BODY_RINT;


	// ---- mcAxisSetSoftlimitMin ----
	M4IAPI int MACH_ENTRY mcAxisSetSoftlimitMin(
		MINSTANCE mInst,
		int axisId,
		double min) DUMMY_BODY_RINT;


	// ---- mcAxisSetSpindle ----
	M4IAPI int MACH_ENTRY mcAxisSetSpindle(
		MINSTANCE mInst,
		int axisId,
		BOOL spindle) DUMMY_BODY_RINT;



	// ---- mcAxisSetVel ----
	M4IAPI int MACH_ENTRY mcAxisSetVel(
		MINSTANCE mInst,
		int axis,
		double velocity) DUMMY_BODY_RINT;


	// ---- mcAxisUnmapMotor ----
	M4IAPI int MACH_ENTRY mcAxisUnmapMotor(
		MINSTANCE mInst,
		int axisId,
		int motor) DUMMY_BODY_RINT;


	// ---- mcAxisUnmapMotors ----
	M4IAPI int MACH_ENTRY mcAxisUnmapMotors(
		MINSTANCE mInst,
		int axisId) DUMMY_BODY_RINT;


	// ---- mcAxisUnregister ----
	M4IAPI int MACH_ENTRY mcAxisUnregister(
		MINSTANCE mInst,
		int axisId) DUMMY_BODY_RINT;


	// ---- mcCntlCheckLicenseFeature ----
	M4IAPI int MACH_ENTRY mcCntlCheckLicenseFeature(
		MINSTANCE  mInst,
		const char *licFile,
		const char *requirement,
		const char *feature) DUMMY_BODY_RINT;


	// ---- mcCntlCleanup ----
	M4IAPI int MACH_ENTRY mcCntlCleanup(MINSTANCE mInst) DUMMY_BODY_RINT;


	// ---- mcCntlCloseGCodeFile ----
	M4IAPI int MACH_ENTRY mcCntlCloseGCodeFile(
		MINSTANCE mInst) DUMMY_BODY_RINT;


	// ---- mcCntlCompileScripts ----
	M4IAPI int MACH_ENTRY mcCntlCompileScripts(
		MINSTANCE mInst) DUMMY_BODY_RINT;


	// ---- mcCntlConfigStart ----
	M4IAPI int MACH_ENTRY mcCntlConfigStart(
		MINSTANCE mInst) DUMMY_BODY_RINT;


	// ---- mcCntlConfigStop ----
	M4IAPI int MACH_ENTRY mcCntlConfigStop(
		MINSTANCE mInst) DUMMY_BODY_RINT;


	// ---- mcCntlConfigStopEx ----
	/* mcCntlConfigStopEx */

	// ---- mcCntlCreateLocalVars ----
	M4IAPI int MACH_ENTRY mcCntlCreateLocalVars(
		MINSTANCE mInst,
		const char *lineParams,
		unsigned long *handle) DUMMY_BODY_RINT;


	// ---- mcCntlCutRecovery ----
	/* mcCntlCutRecovery */

	// ---- mcCntlCutRecoveryCancel ----
	/* mcCntlCutRecoveryCancel */

	// ---- mcCntlCutRecoveryEx ----
	/* mcCntlCutRecoveryEx */

	// ---- mcCntlCycleStart ----
	M4IAPI int MACH_ENTRY mcCntlCycleStart(
		MINSTANCE mInst) DUMMY_BODY_RINT;


	// ---- mcCntlCycleStop ----
	M4IAPI int MACH_ENTRY mcCntlCycleStop(
		MINSTANCE mInst) DUMMY_BODY_RINT;


	// ---- mcCntlDoCode ----
	/* mcCntlDoCode */

	// ---- mcCntlDroGetUseAuxPos ----
	/* mcCntlDroGetUseAuxPos */

	// ---- mcCntlDroGetUseAuxPosAxis ----
	/* mcCntlDroGetUseAuxPosAxis */

	// ---- mcCntlDroRead ----
	/* mcCntlDroRead */

	// ---- mcCntlDroSetUseAuxPos ----
	/* mcCntlDroSetUseAuxPos */

	// ---- mcCntlDroSetUseAuxPosAxis ----
	/* mcCntlDroSetUseAuxPosAxis */

	// ---- mcCntlDroWrite ----
	/* mcCntlDroWrite */

	// ---- mcCntlDryRunToLine ----
	M4IAPI int MACH_ENTRY mcCntlDryRunToLine(
		MINSTANCE mInst,
		int line) DUMMY_BODY_RINT;


	// ---- mcCntlEStop ----
	M4IAPI int MACH_ENTRY mcCntlEStop(
		MINSTANCE mInst) DUMMY_BODY_RINT;


	// ---- mcCntlEnable ----
	M4IAPI int MACH_ENTRY mcCntlEnable(
		MINSTANCE mInst,
		BOOL state) DUMMY_BODY_RINT;


	// ---- mcCntlFeedHold ----
	M4IAPI int MACH_ENTRY mcCntlFeedHold(
		MINSTANCE mInst) DUMMY_BODY_RINT;


	// ---- mcCntlFeedHoldState ----
	M4IAPI int MACH_ENTRY mcCntlFeedHoldState(
		MINSTANCE mInst,
		BOOL *InFeedHold) DUMMY_BODY_RINT;


	// ---- mcCntlGcodeExecute ----
	M4IAPI int MACH_ENTRY mcCntlGcodeExecute(
		MINSTANCE mInst,
		const char *commands) DUMMY_BODY_RINT;


	// ---- mcCntlGcodeExecuteWait ----
	M4IAPI int MACH_ENTRY mcCntlGcodeExecuteWait(
		MINSTANCE mInst,
		const char *commands) DUMMY_BODY_RINT;


	// ---- mcCntlGcodeInterpGetPos ----
	M4IAPI int MACH_ENTRY mcCntlGcodeInterpGetPos(
		MINSTANCE mInst,
		int axisId,
		double *pos) DUMMY_BODY_RINT;


	// ---- mcCntlGetBlockDelete ----
	M4IAPI int MACH_ENTRY mcCntlGetBlockDelete(
		MINSTANCE mInst,
		int deleteId,
		BOOL *val) DUMMY_BODY_RINT;


	// ---- mcCntlGetBuild ----
	M4IAPI int MACH_ENTRY mcCntlGetBuild(
		MINSTANCE mInst,
		char *buf,
		size_t bufsize) DUMMY_BODY_RINT;


	// ---- mcCntlGetComputerID ----
	M4IAPI int MACH_ENTRY mcCntlGetComputerID(
		MINSTANCE mInst,
		char *buf,
		size_t bufSize) DUMMY_BODY_RINT;


	// ---- mcCntlGetCoolantDelay ----
	M4IAPI int MACH_ENTRY mcCntlGetCoolantDelay(
		MINSTANCE mInst,
		double *secs) DUMMY_BODY_RINT;


	// ---- mcCntlGetCutRecoveryPoint ----
	/* mcCntlGetCutRecoveryPoint */

	// ---- mcCntlGetCwd ----
	M4IAPI int MACH_ENTRY mcCntlGetCwd(
		MINSTANCE mInst,
		char *buf,
		size_t bufSize) DUMMY_BODY_RINT;


	// ---- mcCntlGetDiaMode ----
	M4IAPI int MACH_ENTRY mcCntlGetDiaMode(
		MINSTANCE mInst,
		BOOL *dia) DUMMY_BODY_RINT;


	// ---- mcCntlGetDistToGo ----
	M4IAPI int MACH_ENTRY mcCntlGetDistToGo(
		MINSTANCE mInst,
		int axisId,
		double *togo) DUMMY_BODY_RINT;


	// ---- mcCntlGetEnableFRO ----
	M4IAPI int MACH_ENTRY mcCntlGetEnableFRO(
		MINSTANCE mInst,
		BOOL *enable) DUMMY_BODY_RINT;


	// ---- mcCntlGetErrorString ----
	/* mcCntlGetErrorString */

	// ---- mcCntlGetFRO ----
	M4IAPI int MACH_ENTRY mcCntlGetFRO(
		MINSTANCE mInst,
		double *percent) DUMMY_BODY_RINT;


	// ---- mcCntlGetGcodeFileCount ----
	/* mcCntlGetGcodeFileCount */

	// ---- mcCntlGetGcodeFileName ----
	M4IAPI int MACH_ENTRY mcCntlGetGcodeFileName(
		MINSTANCE mInst,
		char *buf,
		size_t bufSize) DUMMY_BODY_RINT;


	// ---- mcCntlGetGcodeLine ----
	M4IAPI int MACH_ENTRY mcCntlGetGcodeLine(
		MINSTANCE mInst,
		int LineNumber,
		char *buf,
		long bufSize) DUMMY_BODY_RINT;


	// ---- mcCntlGetGcodeLineCount ----
	M4IAPI int MACH_ENTRY mcCntlGetGcodeLineCount(
		MINSTANCE mInst,
		double *count) DUMMY_BODY_RINT;


	// ---- mcCntlGetGcodeLineCountEx ----
	/* mcCntlGetGcodeLineCountEx */

	// ---- mcCntlGetGcodeLineNbr ----
	M4IAPI int MACH_ENTRY mcCntlGetGcodeLineNbr(
		MINSTANCE mInst,
		double *val) DUMMY_BODY_RINT;


	// ---- mcCntlGetHdId ----
	/* mcCntlGetHdId */

	// ---- mcCntlGetInstanceHandle ----
	M4IAPI int MACH_ENTRY mcCntlGetInstanceHandle(MINSTANCE mInst,
		const char *owner,
		HMINSTANCE *hInst) DUMMY_BODY_RINT;


	// ---- mcCntlGetLastError ----
	M4IAPI int MACH_ENTRY mcCntlGetLastError(
		MINSTANCE mInst,
		char *buf,
		size_t bufSize) DUMMY_BODY_RINT;


	// ---- mcCntlGetLastLogMsg ----
	M4IAPI int MACH_ENTRY mcCntlGetLastLogMsg(
		MINSTANCE mInst,
		char *buf,
		size_t bufSize) DUMMY_BODY_RINT;


	// ---- mcCntlGetLicenseData ----
	M4IAPI int MACH_ENTRY mcCntlGetLicenseData(
		MINSTANCE mInst,
		int index,
		char *buf,
		long bufSize) DUMMY_BODY_RINT;


	// ---- mcCntlGetLicenseDataLen ----
	M4IAPI int MACH_ENTRY mcCntlGetLicenseDataLen(
		MINSTANCE mInst,
		int index,
		long *bufSize) DUMMY_BODY_RINT;


	// ---- mcCntlGetLicenseModules ----
	M4IAPI int MACH_ENTRY mcCntlGetLicenseModules(
		MINSTANCE mInst,
		unsigned long long *modules) DUMMY_BODY_RINT;


	// ---- mcCntlGetLocalComment ----
	/* mcCntlGetLocalComment */

	// ---- mcCntlGetLocalVar ----
	/* mcCntlGetLocalVar */

	// ---- mcCntlGetLocalVarFlag ----
	M4IAPI int MACH_ENTRY mcCntlGetLocalVarFlag(
		MINSTANCE mInst,
		HMCVARS hVars,
		int varNumber,
		int *retval) DUMMY_BODY_RINT;


	// ---- mcCntlGetLogging ----
	M4IAPI int MACH_ENTRY mcCntlGetLogging(
		MINSTANCE mInst,
		BOOL *enabled) DUMMY_BODY_RINT;


	// ---- mcCntlGetMachDir ----
	M4IAPI int MACH_ENTRY mcCntlGetMachDir(
		MINSTANCE mInst,
		char *buf,
		size_t bufSize) DUMMY_BODY_RINT;


	// ---- mcCntlGetMistDelay ----
	M4IAPI int MACH_ENTRY mcCntlGetMistDelay(
		MINSTANCE mInst,
		double *secs) DUMMY_BODY_RINT;


	// ---- mcCntlGetModalGroup ----
	M4IAPI int MACH_ENTRY mcCntlGetModalGroup(
		MINSTANCE mInst,
		int group,
		double *val) DUMMY_BODY_RINT;


	// ---- mcCntlGetMode ----
	M4IAPI int MACH_ENTRY mcCntlGetMode(
		MINSTANCE mInst,
		int *mode) DUMMY_BODY_RINT;


	// ---- mcCntlGetOffset ----
	M4IAPI int MACH_ENTRY mcCntlGetOffset(
		MINSTANCE mInst,
		int axisId,
		int type,
		double *offset) DUMMY_BODY_RINT;


	// ---- mcCntlGetOptionalStop ----
	M4IAPI int MACH_ENTRY mcCntlGetOptionalStop(
		MINSTANCE mInst,
		BOOL *stop) DUMMY_BODY_RINT;


	// ---- mcCntlGetParameter ----
	/* mcCntlGetParameter */

	// ---- mcCntlGetParameterBit ----
	/* mcCntlGetParameterBit */

	// ---- mcCntlGetPoundVar ----
	M4IAPI int MACH_ENTRY mcCntlGetPoundVar(
		MINSTANCE mInst,
		int param,
		double *value) DUMMY_BODY_RINT;


	// ---- mcCntlGetRRO ----
	M4IAPI int MACH_ENTRY mcCntlGetRRO(
		MINSTANCE mInst,
		double *percent) DUMMY_BODY_RINT;


	// ---- mcCntlGetRunTime ----
	M4IAPI int MACH_ENTRY mcCntlGetRunTime(
		MINSTANCE mInst,
		double *time) DUMMY_BODY_RINT;


	// ---- mcCntlGetSingleBlock ----
	M4IAPI int MACH_ENTRY mcCntlGetSingleBlock(
		MINSTANCE mInst,
		BOOL *sbState) DUMMY_BODY_RINT;


	// ---- mcCntlGetState ----
	M4IAPI int MACH_ENTRY mcCntlGetState(
		MINSTANCE mInst,
		mcState *state) DUMMY_BODY_RINT;


	// ---- mcCntlGetStateName ----
	M4IAPI int MACH_ENTRY mcCntlGetStateName(
		MINSTANCE mInst,
		mcState state,
		char *buf,
		size_t bufSize) DUMMY_BODY_RINT;


	// ---- mcCntlGetToolOffset ----
	M4IAPI int MACH_ENTRY mcCntlGetToolOffset(
		MINSTANCE mInst,
		int axisId,
		double *offset) DUMMY_BODY_RINT;


	// ---- mcCntlGetUnitsCurrent ----
	M4IAPI int MACH_ENTRY mcCntlGetUnitsCurrent(
		MINSTANCE mInst,
		int *units) DUMMY_BODY_RINT;


	// ---- mcCntlGetUnitsDefault ----
	M4IAPI int MACH_ENTRY mcCntlGetUnitsDefault(
		MINSTANCE mInst,
		int *units) DUMMY_BODY_RINT;


	// ---- mcCntlGetValue ----
	M4IAPI int MACH_ENTRY mcCntlGetValue(
		MINSTANCE  mInst,
		int valId,
		int param,
		double *value) DUMMY_BODY_RINT;


	// ---- mcCntlGetVersion ----
	M4IAPI int MACH_ENTRY mcCntlGetVersion(
		MINSTANCE mInst,
		char *buf,
		size_t bufSize) DUMMY_BODY_RINT;


	// ---- mcCntlGotoZero ----
	M4IAPI int MACH_ENTRY mcCntlGotoZero(
		MINSTANCE mInst) DUMMY_BODY_RINT;


	// ---- mcCntlIsInCycle ----
	M4IAPI int MACH_ENTRY mcCntlIsInCycle(
		MINSTANCE mInst,
		BOOL *cycle) DUMMY_BODY_RINT;


	// ---- mcCntlIsStill ----
	M4IAPI int MACH_ENTRY mcCntlIsStill(
		MINSTANCE mInst,
		BOOL *still) DUMMY_BODY_RINT;


	// ---- mcCntlLoadGcodeFile ----
	M4IAPI int MACH_ENTRY mcCntlLoadGcodeFile(
		MINSTANCE mInst,
		const char *FileToLoad) DUMMY_BODY_RINT;


	// ---- mcCntlLoadGcodeString ----
	M4IAPI int MACH_ENTRY mcCntlLoadGcodeString(
		MINSTANCE mInst,
		const char *gCode) DUMMY_BODY_RINT;


	// ---- mcCntlLog ----
	M4IAPI int MACH_ENTRY mcCntlLog(
		MINSTANCE mInst,
		const char *message,
		const char *file,
		int line) DUMMY_BODY_RINT;


	// ---- mcCntlMachineStateClear ----
	M4IAPI int MACH_ENTRY mcCntlMachineStateClear(
		MINSTANCE mInst) DUMMY_BODY_RINT;


	// ---- mcCntlMachineStatePop ----
	M4IAPI int MACH_ENTRY mcCntlMachineStatePop(
		MINSTANCE mInst) DUMMY_BODY_RINT;


	// ---- mcCntlMachineStatePush ----
	M4IAPI int MACH_ENTRY mcCntlMachineStatePush(
		MINSTANCE mInst) DUMMY_BODY_RINT;


	// ---- mcCntlMacroAlarm ----
	M4IAPI int MACH_ENTRY mcCntlMacroAlarm(
		MINSTANCE mInst,
		int error,
		const char *message) DUMMY_BODY_RINT;


	// ---- mcCntlMacroStop ----
	M4IAPI int MACH_ENTRY mcCntlMacroStop(
		MINSTANCE mInst,
		int error,
		const char *message) DUMMY_BODY_RINT;


	// ---- mcCntlMdiExecute ----
	M4IAPI int MACH_ENTRY mcCntlMdiExecute(
		MINSTANCE mInst,
		const char *commands) DUMMY_BODY_RINT;


	// ---- mcCntlProbeFileClose ----
	M4IAPI int MACH_ENTRY mcCntlProbeFileClose(
		MINSTANCE mInst) DUMMY_BODY_RINT;


	// ---- mcCntlProbeFileOpen ----
	M4IAPI int MACH_ENTRY mcCntlProbeFileOpen(
		MINSTANCE mInst,
		const char *fileName,
		const char *format,
		BOOL overWrite) DUMMY_BODY_RINT;


	// ---- mcCntlProbeGetStrikeStatus ----
	M4IAPI int MACH_ENTRY mcCntlProbeGetStrikeStatus(
		MINSTANCE mInst,
		BOOL *didStrike) DUMMY_BODY_RINT;


	// ---- mcCntlReset ----
	M4IAPI int MACH_ENTRY mcCntlReset(
		MINSTANCE mInst) DUMMY_BODY_RINT;


	// ---- mcCntlRewindFile ----
	M4IAPI int MACH_ENTRY mcCntlRewindFile(
		MINSTANCE mInst) DUMMY_BODY_RINT;


	// ---- mcCntlSetBlockDelete ----
	M4IAPI int MACH_ENTRY mcCntlSetBlockDelete(
		MINSTANCE mInst,
		int deleteID,
		BOOL enabled) DUMMY_BODY_RINT;


	// ---- mcCntlSetCoolantDelay ----
	M4IAPI int MACH_ENTRY mcCntlSetCoolantDelay(
		MINSTANCE mInst,
		double secs) DUMMY_BODY_RINT;


	// ---- mcCntlSetDiaMode ----
	M4IAPI int MACH_ENTRY mcCntlSetDiaMode(
		MINSTANCE mInst,
		BOOL enable) DUMMY_BODY_RINT;


	// ---- mcCntlSetEnableFRO ----
	M4IAPI int MACH_ENTRY mcCntlSetEnableFRO(
		MINSTANCE mInst,
		BOOL enable) DUMMY_BODY_RINT;


	// ---- mcCntlSetFRO ----
	M4IAPI int MACH_ENTRY mcCntlSetFRO(
		MINSTANCE mInst,
		double percent) DUMMY_BODY_RINT;


	// ---- mcCntlSetGcodeLineNbr ----
	M4IAPI int MACH_ENTRY mcCntlSetGcodeLineNbr(
		MINSTANCE mInst,
		double line) DUMMY_BODY_RINT;


	// ---- mcCntlSetLastError ----
	M4IAPI int MACH_ENTRY mcCntlSetLastError(
		MINSTANCE mInst,
		const char *emsg) DUMMY_BODY_RINT;


	// ---- mcCntlSetLogging ----
	M4IAPI int MACH_ENTRY mcCntlSetLogging(
		MINSTANCE mInst,
		BOOL enable) DUMMY_BODY_RINT;


	// ---- mcCntlSetMistDelay ----
	M4IAPI int MACH_ENTRY mcCntlSetMistDelay(
		MINSTANCE mInst,
		double secs) DUMMY_BODY_RINT;


	// ---- mcCntlSetMode ----
	M4IAPI int MACH_ENTRY mcCntlSetMode(
		MINSTANCE mInst,
		double mode) DUMMY_BODY_RINT;


	// ---- mcCntlSetOptionalStop ----
	M4IAPI int MACH_ENTRY mcCntlSetOptionalStop(
		MINSTANCE mInst,
		BOOL enable) DUMMY_BODY_RINT;


	// ---- mcCntlSetParameter ----
	/* mcCntlSetParameter */

	// ---- mcCntlSetParameterBit ----
	/* mcCntlSetParameterBit */

	// ---- mcCntlSetPoundVar ----
	M4IAPI int MACH_ENTRY mcCntlSetPoundVar(
		MINSTANCE mInst,
		int param,
		double value) DUMMY_BODY_RINT;


	// ---- mcCntlSetRRO ----
	M4IAPI int MACH_ENTRY mcCntlSetRRO(
		MINSTANCE mInst,
		double percent) DUMMY_BODY_RINT;


	// ---- mcCntlSetResetCodes ----
	M4IAPI int MACH_ENTRY mcCntlSetResetCodes(
		MINSTANCE mInst,
		const char *resetCodes) DUMMY_BODY_RINT;


	// ---- mcCntlSetSingleBlock ----
	M4IAPI int MACH_ENTRY mcCntlSetSingleBlock(
		MINSTANCE mInst,
		BOOL enable) DUMMY_BODY_RINT;


	// ---- mcCntlSetTestReg ----
	/* mcCntlSetTestReg */

	// ---- mcCntlSetValue ----
	M4IAPI int MACH_ENTRY mcCntlSetValue(
		MINSTANCE  mInst,
		int valId,
		int param,
		double value) DUMMY_BODY_RINT;


	// ---- mcCntlStartMotionDev ----
	M4IAPI int MACH_ENTRY mcCntlStartMotionDev(
		MINSTANCE mInst) DUMMY_BODY_RINT;


	// ---- mcCntlStopMotionDev ----
	M4IAPI int MACH_ENTRY mcCntlStopMotionDev(
		MINSTANCE mInst) DUMMY_BODY_RINT;


	// ---- mcCntlToolChangeManual ----
	M4IAPI int MACH_ENTRY mcCntlToolChangeManual(
		MINSTANCE mInst) DUMMY_BODY_RINT;


	// ---- mcCntlWaitOnCycleStart ----
	M4IAPI int MACH_ENTRY mcCntlWaitOnCycleStart(
		MINSTANCE mInst,
		const char *msg,
		int timeOutMs) DUMMY_BODY_RINT;


	// ---- mcCtrlGetRunTime ----
	/* mcCtrlGetRunTime */

	// ---- mcDeviceGetInfo ----
	M4IAPI int MACH_ENTRY mcDeviceGetInfo(
		HMCDEV hDev,
		char *nameBuf,
		size_t nameBuflen,
		char *descBuf,
		size_t descBuflen,
		int *type,
		int *id) DUMMY_BODY_RINT;


	// ---- mcDeviceGetInfoStruct ----

	struct devinfo {
		char devName[80];
		char devDesc[80];
		int  devType;
		int  devId;
	};
	typedef struct devinfo devinfo_t;
	M4IAPI int MACH_ENTRY mcDeviceGetInfoStruct(
		HMCSIG hDev,
		devinfo_t *devinf) DUMMY_BODY_RINT;


	// ---- mcDeviceGetNextHandle ----
	M4IAPI int MACH_ENTRY mcDeviceGetNextHandle(
		MINSTANCE mInst,
		int devtype,
		HMCDEV startDev,
		HMCDEV *hDev) DUMMY_BODY_RINT;


	// ---- mcFeatureIdCreate ----
	/* mcFeatureIdCreate */

	// ---- mcFeatureIdRegister ----
	/* mcFeatureIdRegister */

	// ---- mcFeatureIdUnRegister ----
	/* mcFeatureIdUnRegister */

	// ---- mcFileHoldAquire ----
	M4IAPI int MACH_ENTRY mcFileHoldAquire(
		MINSTANCE mInst,
		const char *reason,
		int JogAxisBits) DUMMY_BODY_RINT;


	// ---- mcFileHoldReason ----
	M4IAPI int MACH_ENTRY mcFileHoldReason(
		MINSTANCE mInst,
		char *buf,
		long bufSize) DUMMY_BODY_RINT;


	// ---- mcFileHoldRelease ----
	M4IAPI int MACH_ENTRY mcFileHoldRelease(
		MINSTANCE mInst) DUMMY_BODY_RINT;


	// ---- mcFixtureLoadFile ----
	M4IAPI int MACH_ENTRY mcFixtureLoadFile(
		MINSTANCE mInst,
		const char *FileToLoad) DUMMY_BODY_RINT;


	// ---- mcFixtureSaveFile ----
	M4IAPI int MACH_ENTRY mcFixtureSaveFile(
		MINSTANCE mInst) DUMMY_BODY_RINT;


	// ---- mcGuiDelCallback ----
	/* mcGuiDelCallback */

	// ---- mcGuiSendMsg ----
	/* mcGuiSendMsg */

	// ---- mcGuiSendSyncMsg ----
	/* mcGuiSendSyncMsg */

	// ---- mcGuiSetCallback ----
	M4IAPI int MACH_ENTRY mcGuiSetCallback(
		MINSTANCE mInst,
		void *fp) DUMMY_BODY_RINT;


	// ---- mcIoGetHandle ----
	M4IAPI int MACH_ENTRY mcIoGetHandle(
		MINSTANCE mInst,
		const char *path,
		HMCIO *hIo) DUMMY_BODY_RINT;


	// ---- mcIoGetInfo ----
	/* mcIoGetInfo */

	// ---- mcIoGetInfoStruct ----

	struct ioinfo {
		char ioName[80];
		char ioDesc[80];
		int  ioType;
		HMCDEV ioDev;
		HMCSIG ioMappedSignals[MAX_MAPPED_SIGNAL];
		void *ioUserData;
		int ioInput;
	};
	typedef struct ioinfo ioinfo_t;M4IAPI int MACH_ENTRY mcIoGetInfoStruct(
		HMCIO hIo,
		ioinfo_t *ioinf) DUMMY_BODY_RINT;


	// ---- mcIoGetNextHandle ----
	M4IAPI int MACH_ENTRY mcIoGetNextHandle(
		HMCDEV hDev,
		HMCIO startIo,
		HMCIO *hIo) DUMMY_BODY_RINT;


	// ---- mcIoGetState ----
	M4IAPI int MACH_ENTRY mcIoGetState(
		HMCIO hIo,
		BOOL *state) DUMMY_BODY_RINT;


	// ---- mcIoGetType ----
	M4IAPI int MACH_ENTRY mcIoGetType(
		HMCIO hIo,
		int *type) DUMMY_BODY_RINT;


	// ---- mcIoIsEnabled ----
	M4IAPI int MACH_ENTRY mcIoIsEnabled(
		HMCIO hIo,
		BOOL *enabled) DUMMY_BODY_RINT;


	// ---- mcIoSetDesc ----
	M4IAPI int MACH_ENTRY mcIoSetDesc(
		HMCIO hIo,
		const char *desc) DUMMY_BODY_RINT;


	// ---- mcIoSetName ----
	M4IAPI int MACH_ENTRY mcIoSetName(
		HMCIO hIo,
		const char *name) DUMMY_BODY_RINT;


	// ---- mcIoSetState ----
	M4IAPI int MACH_ENTRY mcIoSetState(
		HMCIO hIo,
		bool state) DUMMY_BODY_RINT;



	// ---- mcIoSetType ----
	M4IAPI int MACH_ENTRY mcIoSetType(
		HMCIO hIo,
		int type) DUMMY_BODY_RINT;


	// ---- mcIoSyncSignal ----
	M4IAPI int MACH_ENTRY mcIoSyncSignal(
		HMCIO hIo,
		BOOL state) DUMMY_BODY_RINT;


	// ---- mcIpcCleanup ----
	M4IAPI int MACH_ENTRY mcIpcCleanup() DUMMY_BODY_RINT;

	// ---- mcIpcFileDownload ----
	/* mcIpcFileDownload */

	// ---- mcIpcFileUpload ----
	/* mcIpcFileUpload */

	// ---- mcIpcGetLastErrorCode ----
	/* mcIpcGetLastErrorCode */

	// ---- mcIpcGetLastErrorMessage ----
	/* mcIpcGetLastErrorMessage */

	// ---- mcIpcInit ----
	M4IAPI MINSTANCE MACH_ENTRY mcIpcInit(const char *hostname) DUMMY_BODY_RINT;

	// ---- mcJogAbsStart ----
	/* mcJogAbsStart */

	// ---- mcJogAbsStop ----
	/* mcJogAbsStop */

	// ---- mcJogGetAccel ----
	M4IAPI int MACH_ENTRY mcJogGetAccel(
		MINSTANCE mInst,
		int axisId,
		double *percent) DUMMY_BODY_RINT;


	// ---- mcJogGetFeedRate ----
	/* mcJogGetFeedRate */

	// ---- mcJogGetInc ----
	M4IAPI int MACH_ENTRY mcJogGetInc(
		MINSTANCE mInst,
		int axisId,
		double *increment) DUMMY_BODY_RINT;


	// ---- mcJogGetRate ----
	M4IAPI int MACH_ENTRY mcJogGetRate(
		MINSTANCE mInst,
		int axisId,
		double *percent) DUMMY_BODY_RINT;


	// ---- mcJogGetTraceEnable ----
	/* mcJogGetTraceEnable */

	// ---- mcJogGetUnitsMode ----
	/* mcJogGetUnitsMode */

	// ---- mcJogGetVelocity ----
	M4IAPI int MACH_ENTRY mcJogGetVelocity(
		MINSTANCE mInst,
		int axisId,
		double *vel) DUMMY_BODY_RINT;


	// ---- mcJogIncStart ----
	M4IAPI int MACH_ENTRY mcJogIncStart(
		MINSTANCE mInst,
		int axisId,
		double dist) DUMMY_BODY_RINT;


	// ---- mcJogIncStop ----
	M4IAPI int MACH_ENTRY mcJogIncStop(
		MINSTANCE mInst,
		int axisId,
		double incr) DUMMY_BODY_RINT;


	// ---- mcJogIsJogging ----
	M4IAPI int MACH_ENTRY mcJogIsJogging(
		MINSTANCE mInst,
		int axisId,
		BOOL *jogging) DUMMY_BODY_RINT;


	// ---- mcJogIsStopping ----
	M4IAPI int MACH_ENTRY mcJogIsStopping(
		MINSTANCE mInst,
		int axisId,
		BOOL *stopping) DUMMY_BODY_RINT;


	// ---- mcJogSetAccel ----
	M4IAPI int MACH_ENTRY mcJogSetAccel(
		MINSTANCE mInst,
		int axisId,
		double percent) DUMMY_BODY_RINT;


	// ---- mcJogSetFeedRate ----
	/* mcJogSetFeedRate */

	// ---- mcJogSetInc ----
	M4IAPI int MACH_ENTRY mcJogSetInc(
		MINSTANCE mInst,
		int axisId,
		double increment) DUMMY_BODY_RINT;


	// ---- mcJogSetRate ----
	M4IAPI int MACH_ENTRY mcJogSetRate(
		MINSTANCE mInst,
		int axisId,
		double percent) DUMMY_BODY_RINT;


	// ---- mcJogSetTraceEnable ----
	M4IAPI int MACH_ENTRY mcJogSetTraceEnable(
		MINSTANCE mInst,
		BOOL enable) DUMMY_BODY_RINT;


	// ---- mcJogSetType ----
	M4IAPI int MACH_ENTRY mcJogSetType(MINSTANCE mInst, int axisId, int type) DUMMY_BODY_RINT;


	// ---- mcJogSetUnitsMode ----
	/* mcJogSetUnitsMode */

	// ---- mcJogVelocityStart ----
	M4IAPI int MACH_ENTRY mcJogVelocityStart(
		MINSTANCE mInst,
		int axisId,
		double dir) DUMMY_BODY_RINT;


	// ---- mcJogVelocityStop ----
	M4IAPI int MACH_ENTRY mcJogVelocityStop(
		MINSTANCE mInst,
		int axisId) DUMMY_BODY_RINT;


	// ---- mcMotionClearPlanner ----
	M4IAPI int MACH_ENTRY mcMotionClearPlanner(
		MINSTANCE mInst) DUMMY_BODY_RINT;


	// ---- mcMotionGetCVAngle ----
	/* mcMotionGetCVAngle */

	// ---- mcMotionGetCVAngleEnable ----
	/* mcMotionGetCVAngleEnable */

	// ---- mcMotionSetCVAngle ----
	/* mcMotionSetCVAngle */

	// ---- mcMotionSetCVAngleEnable ----
	/* mcMotionSetCVAngleEnable */

	// ---- mcMotionSync ----
	M4IAPI int MACH_ENTRY mcMotionSync(
		MINSTANCE mInst) DUMMY_BODY_RINT;


	// ---- mcMotorGetAxis ----
	M4IAPI int MACH_ENTRY mcMotorGetAxis(
		MINSTANCE mInst,
		int motorId,
		int *axisId) DUMMY_BODY_RINT;


	// ---- mcMotorGetCountsPerUnit ----
	M4IAPI int MACH_ENTRY mcMotorGetCountsPerUnit(
		MINSTANCE mInst,
		int motorId,
		double *counts) DUMMY_BODY_RINT;


	// ---- mcMotorGetInfoStruct ----

	struct motorinfo {
		double CountsPerUnit;    // Number of encoder counts or steps per unit.
		double MaxVelocity;      // Max velocity of the axis.
		double MaxAcceleration;  // Max rate to accelerate.
		BOOL   Reverse;          // Is the axis reversed?
		double BacklashAmount;   // The amount of backlash in counts.
		double CurrentVelocity;  // The speed the axis is moving, This could be reported by the motion deivce.
		int    CurrentPosition;  // The Current Position (From the motion device).
		BOOL   Homed;            // True if the axis has been homed.
		long   SoftMaxLimit;     // Count for the max travel.
		long   SoftMinLimit;     // Count for the min travel.
		BOOL   CanHome;          // Can this motor home?
		BOOL   Enabled;          // Is this motor enabled?
		long   EnableDelay;      // ms to delay the enable signal for this motor.
		int    AxisId;           // -1 if no axis has mapped this motor.
	};
	typedef struct motorinfo motorinfo_t;
	M4IAPI int MACH_ENTRY mcMotorGetInfoStruct(
		MINSTANCE mInst,
		int motorId,
		motorinfo_t *minf) DUMMY_BODY_RINT;


	// ---- mcMotorGetMaxAccel ----
	M4IAPI int MACH_ENTRY mcMotorGetMaxAccel(
		MINSTANCE mInst,
		int motorId,
		double *maxAccel) DUMMY_BODY_RINT;


	// ---- mcMotorGetMaxVel ----
	M4IAPI int MACH_ENTRY mcMotorGetMaxVel(
		MINSTANCE mInst,
		int motorId,
		double *maxVel) DUMMY_BODY_RINT;


	// ---- mcMotorGetPos ----
	M4IAPI int MACH_ENTRY mcMotorGetPos(
		MINSTANCE mInst,
		int motorId,
		double *val) DUMMY_BODY_RINT;


	// ---- mcMotorGetVel ----
	M4IAPI int MACH_ENTRY mcMotorGetVel(
		MINSTANCE mInst,
		int motor,
		double *velocity) DUMMY_BODY_RINT;


	// ---- mcMotorIsHomed ----
	M4IAPI int MACH_ENTRY mcMotorIsHomed(
		MINSTANCE mInst,
		int motorId,
		BOOL *homed) DUMMY_BODY_RINT;


	// ---- mcMotorIsStill ----
	M4IAPI int MACH_ENTRY mcMotorIsStill(
		MINSTANCE mInst,
		int motorId,
		BOOL *still) DUMMY_BODY_RINT;


	// ---- mcMotorMapGetDefinition ----
	M4IAPI int MACH_ENTRY mcMotorMapGetDefinition(
		MINSTANCE mInst,
		int motorId,
		long *lengthCounts,
		long *numPoints) DUMMY_BODY_RINT;


	// ---- mcMotorMapGetEnable ----
	/* mcMotorMapGetEnable */

	// ---- mcMotorMapGetLength ----
	M4IAPI int MACH_ENTRY mcMotorMapGetLength(
		MINSTANCE mInst,
		int motorId,
		int *length) DUMMY_BODY_RINT;


	// ---- mcMotorMapGetNPoints ----
	M4IAPI int MACH_ENTRY mcMotorMapGetNPoints(
		MINSTANCE mInst,
		int motorId,
		int *points) DUMMY_BODY_RINT;


	// ---- mcMotorMapGetPoint ----
	M4IAPI int MACH_ENTRY mcMotorMapGetPoint(
		MINSTANCE mInst,
		int motorId,
		int point,
		int *error) DUMMY_BODY_RINT;


	// ---- mcMotorMapGetPointCount ----
	M4IAPI int MACH_ENTRY mcMotorMapGetPointCount(
		MINSTANCE mInst,
		int motorId,
		int *points) DUMMY_BODY_RINT;


	// ---- mcMotorMapGetStart ----
	M4IAPI int MACH_ENTRY mcMotorMapGetStart(
		MINSTANCE mInst,
		int motorId,
		int *startPoint) DUMMY_BODY_RINT;


	// ---- mcMotorMapSetDefinition ----
	M4IAPI int MACH_ENTRY mcMotorMapSetDefinition(
		MINSTANCE mInst,
		int motorId,
		long lengthCounts,
		long numPoints) DUMMY_BODY_RINT;


	// ---- mcMotorMapSetEnable ----
	/* mcMotorMapSetEnable */

	// ---- mcMotorMapSetLength ----
	M4IAPI int MACH_ENTRY mcMotorMapSetLength(
		MINSTANCE mInst,
		int motorId,
		int length) DUMMY_BODY_RINT;


	// ---- mcMotorMapSetNPoints ----
	M4IAPI int MACH_ENTRY mcMotorMapSetNPoints(
		MINSTANCE mInst,
		int motorId,
		int points) DUMMY_BODY_RINT;


	// ---- mcMotorMapSetPoint ----
	M4IAPI int MACH_ENTRY mcMotorMapSetPoint(
		MINSTANCE mInst,
		int motorId,
		int point,
		int error) DUMMY_BODY_RINT;


	// ---- mcMotorMapSetPointCount ----
	M4IAPI int MACH_ENTRY mcMotorMapSetPointCount(
		MINSTANCE mInst,
		int motorId,
		int points) DUMMY_BODY_RINT;


	// ---- mcMotorMapSetStart ----
	M4IAPI int MACH_ENTRY mcMotorMapSetStart(
		MINSTANCE mInst,
		int motorId,
		int start) DUMMY_BODY_RINT;


	// ---- mcMotorMoveAbs ----
	/* mcMotorMoveAbs */

	// ---- mcMotorSetCountsPerUnit ----
	M4IAPI int MACH_ENTRY mcMotorSetCountsPerUnit(
		MINSTANCE mInst,
		int motorId,
		double counts) DUMMY_BODY_RINT;


	// ---- mcMotorSetHomePos ----
	M4IAPI int MACH_ENTRY mcMotorSetHomePos(
		MINSTANCE mInst,
		int motorId,
		int count) DUMMY_BODY_RINT;


	// ---- mcMotorSetMaxAccel ----
	M4IAPI int MACH_ENTRY mcMotorSetMaxAccel(
		MINSTANCE mInst,
		int motorId,
		double maxAccel) DUMMY_BODY_RINT;


	// ---- mcMotorSetMaxVel ----
	M4IAPI int MACH_ENTRY mcMotorSetMaxVel(
		MINSTANCE mInst,
		int motorId,
		double maxVel) DUMMY_BODY_RINT;


	// ---- mcMotorUnregister ----
	M4IAPI int MACH_ENTRY mcMotorUnregister(
		MINSTANCE mInst,
		int motorId) DUMMY_BODY_RINT;


	// ---- mcMpgGetAccel ----
	M4IAPI int MACH_ENTRY mcMpgGetAccel(
		MINSTANCE mInst,
		int mpg,
		double *percentMaxAccel) DUMMY_BODY_RINT;


	// ---- mcMpgGetAxis ----
	M4IAPI int MACH_ENTRY mcMpgGetAxis(
		MINSTANCE mInst,
		int mpg,
		int *axisId) DUMMY_BODY_RINT;


	// ---- mcMpgGetCountsPerDetent ----
	M4IAPI int MACH_ENTRY mcMpgGetCountsPerDetent(
		MINSTANCE mInst,
		int mpg,
		int *pulses) DUMMY_BODY_RINT;


	// ---- mcMpgGetEnable ----
	/* mcMpgGetEnable */

	// ---- mcMpgGetEncoderReg ----
	M4IAPI int MACH_ENTRY mcMpgGetEncoderReg(
		MINSTANCE mInst,
		int mpg,
		HMCREG *hReg) DUMMY_BODY_RINT;


	// ---- mcMpgGetInc ----
	M4IAPI int MACH_ENTRY mcMpgGetInc(
		MINSTANCE mInst,
		int mpg,
		double *inc) DUMMY_BODY_RINT;


	// ---- mcMpgGetRate ----
	M4IAPI int MACH_ENTRY mcMpgGetRate(
		MINSTANCE mInst,
		int mpg,
		double *percentMaxVel) DUMMY_BODY_RINT;


	// ---- mcMpgGetReversed ----
	/* mcMpgGetReversed */

	// ---- mcMpgGetShuttleMode ----
	M4IAPI int MACH_ENTRY mcMpgGetShuttleMode(
		MINSTANCE mInst,
		BOOL *on) DUMMY_BODY_RINT;


	// ---- mcMpgGetShuttlePercent ----
	/* mcMpgGetShuttlePercent */

	// ---- mcMpgMoveCounts ----
	M4IAPI int MACH_ENTRY mcMpgMoveCounts(
		MINSTANCE mInst,
		int mpg,
		int deltaCounts) DUMMY_BODY_RINT;


	// ---- mcMpgSetAccel ----
	M4IAPI int MACH_ENTRY mcMpgSetAccel(
		MINSTANCE mInst,
		int mpg,
		double percentMaxAccel) DUMMY_BODY_RINT;


	// ---- mcMpgSetAxis ----
	M4IAPI int MACH_ENTRY mcMpgSetAxis(
		MINSTANCE mInst,
		int mpg,
		int axisId) DUMMY_BODY_RINT;


	// ---- mcMpgSetCountsPerDetent ----
	M4IAPI int MACH_ENTRY mcMpgSetCountsPerDetent(
		MINSTANCE mInst,
		int mpg,
		int pulses) DUMMY_BODY_RINT;


	// ---- mcMpgSetEnable ----
	/* mcMpgSetEnable */

	// ---- mcMpgSetEncoderReg ----
	M4IAPI int MACH_ENTRY mcMpgSetEncoderReg(
		MINSTANCE mInst,
		int mpg,
		HMCREG hReg) DUMMY_BODY_RINT;


	// ---- mcMpgSetInc ----
	M4IAPI int MACH_ENTRY mcMpgSetInc(
		MINSTANCE mInst,
		int mpg,
		double inc) DUMMY_BODY_RINT;


	// ---- mcMpgSetRate ----
	M4IAPI int MACH_ENTRY mcMpgSetRate(
		MINSTANCE mInst,
		int mpg,
		double percentMaxVel) DUMMY_BODY_RINT;


	// ---- mcMpgSetReversed ----
	/* mcMpgSetReversed */

	// ---- mcMpgSetShuttleMode ----
	M4IAPI int MACH_ENTRY mcMpgSetShuttleMode(
		MINSTANCE mInst,
		BOOL on) DUMMY_BODY_RINT;


	// ---- mcMpgSetShuttlePercent ----
	/* mcMpgSetShuttlePercent */

	// ---- mcPluginConfigure ----
	M4IAPI int MACH_ENTRY mcPluginConfigure(
		MINSTANCE mInst,
		int plugId) DUMMY_BODY_RINT;


	// ---- mcPluginDiagnostic ----
	M4IAPI int MACH_ENTRY mcPluginDiagnostic(
		MINSTANCE mInst,
		int pluginId) DUMMY_BODY_RINT;


	// ---- mcPluginSendGuiContextMsg ----
	/* mcPluginSendGuiContextMsg */

	// ---- mcProfileDeleteKey ----
	M4IAPI int MACH_ENTRY mcProfileDeleteKey(
		MINSTANCE mInst,
		const char *section,
		const char *key) DUMMY_BODY_RINT;


	// ---- mcProfileDeleteSection ----
	M4IAPI int MACH_ENTRY mcProfileDeleteSection(
		MINSTANCE mInst,
		const char *section) DUMMY_BODY_RINT;


	// ---- mcProfileEnumerateSection ----
	/* mcProfileEnumerateSection */

	// ---- mcProfileExists ----
	M4IAPI int MACH_ENTRY mcProfileExists(
		MINSTANCE mInst,
		const char *section,
		const char *key) DUMMY_BODY_RINT;


	// ---- mcProfileFlush ----
	M4IAPI int MACH_ENTRY mcProfileFlush(
		MINSTANCE mInst) DUMMY_BODY_RINT;


	// ---- mcProfileGetDouble ----
	M4IAPI int MACH_ENTRY mcProfileGetDouble(
		MINSTANCE mInst,
		const char *section,
		const char *key,
		double *retval,
		double defval) DUMMY_BODY_RINT;


	// ---- mcProfileGetInt ----
	M4IAPI int MACH_ENTRY mcProfileGetInt(
		MINSTANCE mInst,
		const char *section,
		const char *key,
		long *retval,
		long defval) DUMMY_BODY_RINT;


	// ---- mcProfileGetName ----
	M4IAPI int MACH_ENTRY mcProfileGetName(
		MINSTANCE mInst,
		char *buff,
		size_t bufsize) DUMMY_BODY_RINT;


	// ---- mcProfileGetString ----
	M4IAPI int MACH_ENTRY mcProfileGetString(
		MINSTANCE mInst,
		const char *section,
		const char *key,
		char *buff,
		long buffsize,
		const char *defval) DUMMY_BODY_RINT;


	// ---- mcProfileReload ----
	M4IAPI int MACH_ENTRY mcProfileReload(
		MINSTANCE mInst) DUMMY_BODY_RINT;


	// ---- mcProfileSave ----
	M4IAPI int MACH_ENTRY mcProfileSave(
		MINSTANCE mInst) DUMMY_BODY_RINT;


	// ---- mcProfileWriteDouble ----
	M4IAPI int MACH_ENTRY mcProfileWriteDouble(
		MINSTANCE mInst,
		const char *section,
		const char *key,
		double val) DUMMY_BODY_RINT;


	// ---- mcProfileWriteInt ----
	M4IAPI int MACH_ENTRY mcProfileWriteInt(
		MINSTANCE mInst,
		const char *section,
		const char *key,
		long val) DUMMY_BODY_RINT;


	// ---- mcProfileWriteString ----
	M4IAPI int MACH_ENTRY mcProfileWriteString(
		MINSTANCE mInst,
		const char *section,
		const char *key,
		const char *val) DUMMY_BODY_RINT;


	// ---- mcRegGetHandle ----
	M4IAPI int MACH_ENTRY mcRegGetHandle(
		MINSTANCE mInst,
		const char *path,
		HMCREG *hReg) DUMMY_BODY_RINT;


	// ---- mcRegGetInfo ----
	M4IAPI int MACH_ENTRY mcRegGetInfo(
		HMCREG hReg,
		char *nameBuf,
		size_t nameBuflen,
		char *descBuf,
		size_t descBuflen,
		int *type,
		HMCDEV *hDev) DUMMY_BODY_RINT;


	// ---- mcRegGetInfoStruct ----

	struct reginfo {
		char regName[80];
		char regDesc[80];
		int  regType;
		HMCDEV regDev;
		void *regUserData;
		int regInput;
	};
	typedef struct reginfo reginfo_t;
	M4IAPI int MACH_ENTRY mcRegGetInfoStruct(
		HMCREG hReg,
		reginfo_t *reginf) DUMMY_BODY_RINT;


	// ---- mcRegGetNextHandle ----
	M4IAPI int MACH_ENTRY mcRegGetNextHandle(
		HMCDEV hDev,
		HMCREG startReg,
		HMCREG *hReg) DUMMY_BODY_RINT;


	// ---- mcRegGetValue ----
	M4IAPI int MACH_ENTRY mcRegGetValue(
		HMCREG hReg,
		double *value) DUMMY_BODY_RINT;


	// ---- mcRegGetValueLong ----
	M4IAPI int MACH_ENTRY mcRegGetValueLong(
		HMCREG hReg,
		double *value) DUMMY_BODY_RINT;


	// ---- mcRegGetValueString ----
	M4IAPI int MACH_ENTRY mcRegGetValueString(
		HMCREG hReg,
		char *buf,
		size_t bufSize) DUMMY_BODY_RINT;


	// ---- mcRegGetValueStringClear ----
	M4IAPI int MACH_ENTRY mcRegGetValueStringClear(
		HMCREG hReg,
		char *buf,
		size_t bufSize) DUMMY_BODY_RINT;


	// ---- mcRegSendCommand ----
	M4IAPI int MACH_ENTRY mcRegSendCommand(
		HMCREG hReg,
		const char *command,
		char *response,
		size_t responseLen) DUMMY_BODY_RINT;


	// ---- mcRegSetDesc ----
	M4IAPI int MACH_ENTRY mcRegSetDesc(
		HMCREG hReg,
		const char *desc) DUMMY_BODY_RINT;


	// ---- mcRegSetName ----
	M4IAPI int MACH_ENTRY mcRegSetName(
		HMCREG hReg,
		const char *name) DUMMY_BODY_RINT;


	// ---- mcRegSetType ----
	M4IAPI int MACH_ENTRY mcRegSetType(
		HMCREG hReg,
		int regType) DUMMY_BODY_RINT;


	// ---- mcRegSetValue ----
	M4IAPI int MACH_ENTRY mcRegSetValue(
		HMCREG hReg,
		double value) DUMMY_BODY_RINT;


	// ---- mcRegSetValueLong ----
	M4IAPI int MACH_ENTRY mcRegSetValueLong(
		HMCREG hReg,
		double value) DUMMY_BODY_RINT;


	// ---- mcRegSetValueString ----
	M4IAPI int MACH_ENTRY mcRegSetValueString(
		HMCREG hReg,
		const char *value) DUMMY_BODY_RINT;


	// ---- mcScriptExecute ----
	M4IAPI int MACH_ENTRY mcScriptExecute(
		MINSTANCE mInst,
		const char *filename,
		BOOL async) DUMMY_BODY_RINT;


	// ---- mcScriptExecuteIfExists ----
	M4IAPI int MACH_ENTRY mcScriptExecuteIfExists(
		MINSTANCE mInst,
		const char *filename,
		BOOL async) DUMMY_BODY_RINT;


	// ---- mcScriptExecutePrivate ----
	M4IAPI int MACH_ENTRY mcScriptExecutePrivate(
		MINSTANCE mInst,
		const char *filename,
		BOOL async) DUMMY_BODY_RINT;


	// ---- mcSignalEnable ----
	M4IAPI int MACH_ENTRY mcSignalEnable(
		HMCSIG hSig,
		BOOL enabled) DUMMY_BODY_RINT;


	// ---- mcSignalGetHandle ----
	M4IAPI int MACH_ENTRY mcSignalGetHandle(
		MINSTANCE mInst,
		int sigid,
		HMCSIG *hSig) DUMMY_BODY_RINT;


	// ---- mcSignalGetInfo ----
	M4IAPI int MACH_ENTRY mcSignalGetInfo(
		HMCSIG hSig,
		int *enabled,
		char *name,
		size_t namelen,
		char *desc,
		size_t desclen,
		int *activelow) DUMMY_BODY_RINT;


	// ---- mcSignalGetInfoEx ----
	/* mcSignalGetInfoEx */

	// ---- mcSignalGetInfoStruct ----

	struct siginfo {
		char sigName[80];
		char sigDesc[80];
		int  sigEnabled;
		int  sigActiveLow;
		HMCIO sigMappedIoHandle;
	};
	typedef struct siginfo siginfo_t;
	M4IAPI int MACH_ENTRY mcSignalGetInfoStruct(
		HMCSIG hSig,
		siginfo_t *siginf) DUMMY_BODY_RINT;


	// ---- mcSignalGetNextHandle ----
	M4IAPI int MACH_ENTRY mcSignalGetNextHandle(
		MINSTANCE mInst,
		int sigtype,
		HMCSIG startSig,
		HMCSIG *hSig) DUMMY_BODY_RINT;


	// ---- mcSignalGetState ----
	M4IAPI int MACH_ENTRY mcSignalGetState(
		HMCSIG hSig,
		BOOL *state) DUMMY_BODY_RINT;


	// ---- mcSignalHandleWait ----
	M4IAPI int MACH_ENTRY mcSignalHandleWait(
		HMCSIG hSig,
		int waitMode,
		double timeoutSecs) DUMMY_BODY_RINT;


	// ---- mcSignalMap ----
	M4IAPI int MACH_ENTRY mcSignalMap(
		HMCSIG hSig,
		HMCIO hIo) DUMMY_BODY_RINT;


	// ---- mcSignalSetActiveLow ----
	M4IAPI int MACH_ENTRY mcSignalSetActiveLow(
		HMCSIG hSig,
		BOOL activelow) DUMMY_BODY_RINT;


	// ---- mcSignalSetState ----
	M4IAPI int MACH_ENTRY mcSignalSetState(
		HMCSIG hSig,
		BOOL enabled) DUMMY_BODY_RINT;


	// ---- mcSignalUnmap ----
	M4IAPI int MACH_ENTRY mcSignalUnmap(
		HMCSIG hSig) DUMMY_BODY_RINT;


	// ---- mcSignalWait ----
	M4IAPI int MACH_ENTRY mcSignalWait(
		MINSTANCE mInst,
		int sigId,
		int waitMode,
		double timeoutSecs) DUMMY_BODY_RINT;


	// ---- mcSoftLimitGetState ----
	M4IAPI int MACH_ENTRY mcSoftLimitGetState(
		MINSTANCE mInst,
		int axis,
		double *ison) DUMMY_BODY_RINT;


	// ---- mcSoftLimitMaxMinsClear ----
	M4IAPI int MACH_ENTRY mcSoftLimitMaxMinsClear(
		MINSTANCE mInst) DUMMY_BODY_RINT;


	// ---- mcSoftLimitSetState ----
	M4IAPI int MACH_ENTRY mcSoftLimitSetState(
		MINSTANCE mInst,
		int axis,
		int on) DUMMY_BODY_RINT;


	// ---- mcSpindleCalcCSSToRPM ----
	M4IAPI int MACH_ENTRY mcSpindleCalcCSSToRPM(
		MINSTANCE mInst,
		double DiaOfCut,
		BOOL Inch) DUMMY_BODY_RINT;


	// ---- mcSpindleGetAccelTime ----
	M4IAPI int MACH_ENTRY mcSpindleGetAccelTime(
		MINSTANCE mInst,
		int Range,
		double *Sec) DUMMY_BODY_RINT;


	// ---- mcSpindleGetCommandRPM ----
	M4IAPI int MACH_ENTRY mcSpindleGetCommandRPM(
		MINSTANCE mInst,
		double *RPM) DUMMY_BODY_RINT;


	// ---- mcSpindleGetCurrentRange ----
	M4IAPI int MACH_ENTRY mcSpindleGetCurrentRange(
		MINSTANCE mInst,
		int *Range) DUMMY_BODY_RINT;


	// ---- mcSpindleGetDecelTime ----
	M4IAPI int MACH_ENTRY mcSpindleGetDecelTime(
		MINSTANCE mInst,
		int Range,
		double *Sec) DUMMY_BODY_RINT;


	// ---- mcSpindleGetDirection ----
	M4IAPI int MACH_ENTRY mcSpindleGetDirection(
		MINSTANCE mInst,
		int *dir) DUMMY_BODY_RINT;


	// ---- mcSpindleGetFeedbackRatio ----
	M4IAPI int MACH_ENTRY mcSpindleGetFeedbackRatio(
		MINSTANCE mInst,
		int Range,
		double *Ratio) DUMMY_BODY_RINT;


	// ---- mcSpindleGetMaxRPM ----
	M4IAPI int MACH_ENTRY mcSpindleGetMaxRPM(
		MINSTANCE mInst,
		int Range,
		double *MaxRPM) DUMMY_BODY_RINT;


	// ---- mcSpindleGetMinRPM ----
	M4IAPI int MACH_ENTRY mcSpindleGetMinRPM(
		MINSTANCE mInst,
		int Range,
		double *MinRPM) DUMMY_BODY_RINT;


	// ---- mcSpindleGetMotorAccel ----
	M4IAPI int MACH_ENTRY mcSpindleGetMotorAccel(
		MINSTANCE mInst,
		int Range,
		double *Accel) DUMMY_BODY_RINT;


	// ---- mcSpindleGetMotorMaxRPM ----
	M4IAPI int MACH_ENTRY mcSpindleGetMotorMaxRPM(
		MINSTANCE mInst,
		double *RPM) DUMMY_BODY_RINT;


	// ---- mcSpindleGetMotorRPM ----
	M4IAPI int MACH_ENTRY mcSpindleGetMotorRPM(
		MINSTANCE mInst,
		double *RPM) DUMMY_BODY_RINT;


	// ---- mcSpindleGetOverride ----
	M4IAPI int MACH_ENTRY mcSpindleGetOverride(
		MINSTANCE mInst,
		double *percent) DUMMY_BODY_RINT;


	// ---- mcSpindleGetOverrideEnable ----
	M4IAPI int MACH_ENTRY mcSpindleGetOverrideEnable(
		MINSTANCE mInst,
		BOOL *enabled) DUMMY_BODY_RINT;


	// ---- mcSpindleGetReverse ----
	M4IAPI int MACH_ENTRY mcSpindleGetReverse(
		MINSTANCE mInst,
		int Range,
		BOOL *Reversed) DUMMY_BODY_RINT;


	// ---- mcSpindleGetSensorRPM ----
	M4IAPI int MACH_ENTRY mcSpindleGetSensorRPM(
		MINSTANCE mInst,
		double *RPM) DUMMY_BODY_RINT;


	// ---- mcSpindleGetSpeedCheckEnable ----
	M4IAPI int MACH_ENTRY mcSpindleGetSpeedCheckEnable(
		MINSTANCE mInst,
		BOOL *enabled) DUMMY_BODY_RINT;


	// ---- mcSpindleGetSpeedCheckPercent ----
	M4IAPI int MACH_ENTRY mcSpindleGetSpeedCheckPercent(
		MINSTANCE mInst,
		double *percent) DUMMY_BODY_RINT;


	// ---- mcSpindleGetTrueRPM ----
	M4IAPI int MACH_ENTRY mcSpindleGetTrueRPM(
		MINSTANCE mInst,
		double *RPM) DUMMY_BODY_RINT;


	// ---- mcSpindleSetAccelTime ----
	M4IAPI int MACH_ENTRY mcSpindleSetAccelTime(
		MINSTANCE mInst,
		int Range,
		double Sec) DUMMY_BODY_RINT;


	// ---- mcSpindleSetCommandRPM ----
	M4IAPI int MACH_ENTRY mcSpindleSetCommandRPM(
		MINSTANCE mInst,
		double RPM) DUMMY_BODY_RINT;


	// ---- mcSpindleSetDecelTime ----
	M4IAPI int MACH_ENTRY mcSpindleSetDecelTime(
		MINSTANCE mInst,
		int Range,
		double Sec) DUMMY_BODY_RINT;


	// ---- mcSpindleSetDirection ----
	M4IAPI int MACH_ENTRY mcSpindleSetDirection(
		MINSTANCE mInst,
		int dir) DUMMY_BODY_RINT;


	// ---- mcSpindleSetDirectionWait ----
	M4IAPI int MACH_ENTRY mcSpindleSetDirectionWait(
		MINSTANCE mInst,
		int dir) DUMMY_BODY_RINT;


	// ---- mcSpindleSetFeedbackRatio ----
	M4IAPI int MACH_ENTRY mcSpindleSetFeedbackRatio(
		MINSTANCE mInst,
		int Range,
		double Ratio) DUMMY_BODY_RINT;


	// ---- mcSpindleSetMaxRPM ----
	M4IAPI int MACH_ENTRY mcSpindleSetMaxRPM(
		MINSTANCE mInst,
		int Range,
		double RPM) DUMMY_BODY_RINT;


	// ---- mcSpindleSetMinRPM ----
	M4IAPI int MACH_ENTRY mcSpindleSetMinRPM(
		MINSTANCE mInst,
		int Range,
		double RPM) DUMMY_BODY_RINT;


	// ---- mcSpindleSetMotorAccel ----
	M4IAPI int MACH_ENTRY mcSpindleSetMotorAccel(
		MINSTANCE mInst,
		int Range,
		double Accel) DUMMY_BODY_RINT;


	// ---- mcSpindleSetMotorMaxRPM ----
	M4IAPI int MACH_ENTRY mcSpindleSetMotorMaxRPM(
		MINSTANCE mInst,
		double RPM) DUMMY_BODY_RINT;


	// ---- mcSpindleSetOverride ----
	M4IAPI int MACH_ENTRY mcSpindleSetOverride(
		MINSTANCE mInst,
		double percent) DUMMY_BODY_RINT;


	// ---- mcSpindleSetOverrideEnable ----
	M4IAPI int MACH_ENTRY mcSpindleSetOverrideEnable(
		MINSTANCE mInst,
		BOOL enable) DUMMY_BODY_RINT;


	// ---- mcSpindleSetRange ----
	M4IAPI int MACH_ENTRY mcSpindleSetRange(
		MINSTANCE mInst,
		int Range) DUMMY_BODY_RINT;


	// ---- mcSpindleSetReverse ----
	M4IAPI int MACH_ENTRY mcSpindleSetReverse(
		MINSTANCE mInst,
		int Range,
		BOOL Reversed) DUMMY_BODY_RINT;


	// ---- mcSpindleSetSensorRPM ----
	M4IAPI int MACH_ENTRY mcSpindleSetSensorRPM(
		MINSTANCE mInst,
		double RPM) DUMMY_BODY_RINT;


	// ---- mcSpindleSetSpeedCheckEnable ----
	M4IAPI int MACH_ENTRY mcSpindleSetSpeedCheckEnable(
		MINSTANCE mInst,
		BOOL enable) DUMMY_BODY_RINT;


	// ---- mcSpindleSetSpeedCheckPercent ----
	M4IAPI int MACH_ENTRY mcSpindleSetSpeedCheckPercent(
		MINSTANCE mInst,
		double percent) DUMMY_BODY_RINT;


	// ---- mcSpindleSpeedCheck ----
	M4IAPI int MACH_ENTRY mcSpindleSpeedCheck(
		MINSTANCE mInst,
		int *SpeedOk) DUMMY_BODY_RINT;


	// ---- mcSpindleWaitForSpeed ----
	/* mcSpindleWaitForSpeed */

	// ---- mcSpindleWaitForStop ----
	/* mcSpindleWaitForStop */

	// ---- mcTlmAddOrChangeGroup ----
	/* mcTlmAddOrChangeGroup */

	// ---- mcTlmAddOrChangeTool ----
	/* mcTlmAddOrChangeTool */

	// ---- mcTlmDeleteGroup ----
	/* mcTlmDeleteGroup */

	// ---- mcTlmDeleteGroups ----
	/* mcTlmDeleteGroups */

	// ---- mcTlmDeleteTool ----
	/* mcTlmDeleteTool */

	// ---- mcTlmDeleteTools ----
	/* mcTlmDeleteTools */

	// ---- mcTlmGetCountOverrideEnable ----
	/* mcTlmGetCountOverrideEnable */

	// ---- mcTlmGetCountOverridePercent ----
	/* mcTlmGetCountOverridePercent */

	// ---- mcTlmGetCurrentGroup ----
	/* mcTlmGetCurrentGroup */

	// ---- mcTlmGetNextGroup ----
	/* mcTlmGetNextGroup */

	// ---- mcTlmGetSelectedGroup ----
	/* mcTlmGetSelectedGroup */

	// ---- mcTlmGroupGetInfo ----
	/* mcTlmGroupGetInfo */

	// ---- mcTlmGroupGetNext ----
	/* mcTlmGroupGetNext */

	// ---- mcTlmGroupGetPrev ----
	/* mcTlmGroupGetPrev */

	// ---- mcTlmLoadFile ----
	/* mcTlmLoadFile */

	// ---- mcTlmResetToolData ----
	/* mcTlmResetToolData */

	// ---- mcTlmSaveFile ----
	/* mcTlmSaveFile */

	// ---- mcTlmSetCountOverrideEnable ----
	/* mcTlmSetCountOverrideEnable */

	// ---- mcTlmSetCountOverridePercent ----
	/* mcTlmSetCountOverridePercent */

	// ---- mcTlmToolGetInfo ----
	/* mcTlmToolGetInfo */

	// ---- mcTlmToolGetNext ----
	/* mcTlmToolGetNext */

	// ---- mcTlmToolGetPrev ----
	/* mcTlmToolGetPrev */

	// ---- mcTlmToolIsManaged ----
	/* mcTlmToolIsManaged */

	// ---- mcTlmToolSkip ----
	/* mcTlmToolSkip */

	// ---- mcTlmToolSkipReset ----
	/* mcTlmToolSkipReset */

	// ---- mcToolGetCurrent ----
	M4IAPI int MACH_ENTRY mcToolGetCurrent(
		MINSTANCE mInst,
		int *toolnum) DUMMY_BODY_RINT;


	// ---- mcToolGetData ----
	M4IAPI int MACH_ENTRY mcToolGetData(
		MINSTANCE mInst,
		int type,
		int toolNumber,
		double *value) DUMMY_BODY_RINT;


	// ---- mcToolGetDataExDbl ----
	M4IAPI int MACH_ENTRY mcToolGetDataExDbl(
		MINSTANCE mInst,
		int toolnum,
		const char *fieldName,
		double *value) DUMMY_BODY_RINT;


	// ---- mcToolGetDataExInt ----
	M4IAPI int MACH_ENTRY mcToolGetDataExInt(
		MINSTANCE mInst,
		int toolnum,
		const char *fieldName,
		int *value) DUMMY_BODY_RINT;


	// ---- mcToolGetDataExStr ----
	M4IAPI int MACH_ENTRY mcToolGetDataExStr(
		MINSTANCE mInst,
		int toolnum,
		const char *fieldName,
		char *value,
		size_t valueLen) DUMMY_BODY_RINT;


	// ---- mcToolGetDesc ----
	M4IAPI int MACH_ENTRY mcToolGetDesc(
		MINSTANCE mInst,
		int toolnum,
		char *buff,
		size_t bufsize) DUMMY_BODY_RINT;


	// ---- mcToolGetPropertyDbl ----
	/* mcToolGetPropertyDbl */

	// ---- mcToolGetPropertyInt ----
	/* mcToolGetPropertyInt */

	// ---- mcToolGetPropertyStr ----
	/* mcToolGetPropertyStr */

	// ---- mcToolGetSelected ----
	M4IAPI int MACH_ENTRY mcToolGetSelected(
		MINSTANCE mInst,
		int *toolnum) DUMMY_BODY_RINT;


	// ---- mcToolLoadFile ----
	M4IAPI int MACH_ENTRY mcToolLoadFile(
		MINSTANCE mInst,
		const char *FileToLoad) DUMMY_BODY_RINT;


	// ---- mcToolPathCreate ----
	M4IAPI int MACH_ENTRY mcToolPathCreate(
		MINSTANCE mInst,
		void *window) DUMMY_BODY_RINT;


	// ---- mcToolPathDelete ----
	M4IAPI int MACH_ENTRY mcToolPathDelete(
		MINSTANCE mInst,
		void *parent) DUMMY_BODY_RINT;


	// ---- mcToolPathGenerate ----
	M4IAPI int MACH_ENTRY mcToolPathGenerate(
		MINSTANCE mInst) DUMMY_BODY_RINT;


	// ---- mcToolPathGenerateAbort ----
	M4IAPI int MACH_ENTRY mcToolPathGenerateAbort(
		MINSTANCE mInst) DUMMY_BODY_RINT;


	// ---- mcToolPathGeneratedPercent ----
	M4IAPI int MACH_ENTRY mcToolPathGeneratedPercent(
		MINSTANCE mInst,
		double *percent) DUMMY_BODY_RINT;


	// ---- mcToolPathGetAAxisPosition ----
	M4IAPI int MACH_ENTRY mcToolPathGetAAxisPosition(MINSTANCE mInst, double *xPos, double *yPos, double *zPos) DUMMY_BODY_RINT;


	// ---- mcToolPathGetARotationAxis ----
	M4IAPI int MACH_ENTRY mcToolPathGetARotationAxis(MINSTANCE mInst, int *axis) DUMMY_BODY_RINT;


	// ---- mcToolPathGetAxisColor ----
	M4IAPI int MACH_ENTRY mcToolPathGetAxisColor(
		MINSTANCE mInst,
		unsigned long *axiscolor,
		unsigned long *limitcolor) DUMMY_BODY_RINT;


	// ---- mcToolPathGetBackColor ----
	M4IAPI int MACH_ENTRY mcToolPathGetBackColor(
		MINSTANCE mInst,
		unsigned long *topcolor,
		unsigned long *botcolor) DUMMY_BODY_RINT;


	// ---- mcToolPathGetDrawLimits ----
	M4IAPI int MACH_ENTRY mcToolPathGetDrawLimits(
		MINSTANCE mInst,
		BOOL *drawlimits) DUMMY_BODY_RINT;


	// ---- mcToolPathGetFollowMode ----
	M4IAPI int MACH_ENTRY mcToolPathGetFollowMode(
		MINSTANCE mInst,
		BOOL *enabled) DUMMY_BODY_RINT;


	// ---- mcToolPathGetGenerating ----
	M4IAPI int MACH_ENTRY mcToolPathGetGenerating(
		MINSTANCE mInst,
		int *pathGenerating) DUMMY_BODY_RINT;


	// ---- mcToolPathGetLeftMouseDn ----
	M4IAPI int MACH_ENTRY mcToolPathGetLeftMouseDn(
		MINSTANCE mInst,
		double *x,
		double *y,
		double *z) DUMMY_BODY_RINT;


	// ---- mcToolPathGetLeftMouseUp ----
	M4IAPI int MACH_ENTRY mcToolPathGetLeftMouseUp(
		MINSTANCE mInst,
		double *x,
		double *y,
		double *z) DUMMY_BODY_RINT;


	// ---- mcToolPathGetMouseLeftClick ----
	/* mcToolPathGetMouseLeftClick */

	// ---- mcToolPathGetPathColor ----
	M4IAPI int MACH_ENTRY mcToolPathGetPathColor(
		MINSTANCE mInst,
		unsigned long *rapidcolor,
		unsigned long *linecolor,
		unsigned long *arccolor,
		unsigned long *highlightcolor) DUMMY_BODY_RINT;


	// ---- mcToolPathGetUseAuxPos ----
	/* mcToolPathGetUseAuxPos */

	// ---- mcToolPathGetUseAuxPosAxis ----
	/* mcToolPathGetUseAuxPosAxis */

	// ---- mcToolPathIsSignalMouseClicks ----
	M4IAPI int MACH_ENTRY mcToolPathIsSignalMouseClicks(
		MINSTANCE mInst,
		BOOL *enabled) DUMMY_BODY_RINT;


	// ---- mcToolPathSetAAxisPosition ----
	M4IAPI int MACH_ENTRY mcToolPathSetAAxisPosition(MINSTANCE mInst, double xPos, double yPos, double zPos) DUMMY_BODY_RINT;


	// ---- mcToolPathSetARotationAxis ----
	M4IAPI int MACH_ENTRY mcToolPathSetARotationAxis(MINSTANCE mInst, int axis) DUMMY_BODY_RINT;


	// ---- mcToolPathSetAxisColor ----
	M4IAPI int MACH_ENTRY mcToolPathSetAxisColor(
		MINSTANCE mInst,
		unsigned long axiscolor,
		unsigned long limitcolor) DUMMY_BODY_RINT;


	// ---- mcToolPathSetBackColor ----
	M4IAPI int MACH_ENTRY mcToolPathSetBackColor(
		MINSTANCE mInst,
		unsigned long topcolor,
		unsigned long botcolor) DUMMY_BODY_RINT;


	// ---- mcToolPathSetDrawLimits ----
	M4IAPI int MACH_ENTRY mcToolPathSetDrawLimits(
		MINSTANCE mInst,
		BOOL drawlimits) DUMMY_BODY_RINT;


	// ---- mcToolPathSetFollowMode ----
	M4IAPI int MACH_ENTRY mcToolPathSetFollowMode(
		MINSTANCE mInst,
		BOOL enabled) DUMMY_BODY_RINT;


	// ---- mcToolPathSetMouseLeftClick ----
	/* mcToolPathSetMouseLeftClick */

	// ---- mcToolPathSetPathColor ----
	M4IAPI int MACH_ENTRY mcToolPathSetPathColor(
		MINSTANCE mInst,
		unsigned long rapidcolor,
		unsigned long linecolor,
		unsigned long arccolor,
		unsigned long highlightcolor) DUMMY_BODY_RINT;


	// ---- mcToolPathSetSignalMouseClicks ----
	M4IAPI int MACH_ENTRY mcToolPathSetSignalMouseClicks(
		MINSTANCE mInst,
		BOOL enabled) DUMMY_BODY_RINT;


	// ---- mcToolPathSetUseAuxPos ----
	/* mcToolPathSetUseAuxPos */

	// ---- mcToolPathSetUseAuxPosAxis ----
	/* mcToolPathSetUseAuxPosAxis */

	// ---- mcToolPathUpdate ----
	M4IAPI int MACH_ENTRY mcToolPathUpdate(
		MINSTANCE mInst,
		void *parent) DUMMY_BODY_RINT;


	// ---- mcToolSaveFile ----
	M4IAPI int MACH_ENTRY mcToolSaveFile(
		MINSTANCE mInst) DUMMY_BODY_RINT;


	// ---- mcToolSetCurrent ----
	M4IAPI int MACH_ENTRY mcToolSetCurrent(
		MINSTANCE mInst,
		int toolnum) DUMMY_BODY_RINT;


	// ---- mcToolSetData ----
	M4IAPI int MACH_ENTRY mcToolSetData(
		MINSTANCE mInst,
		int Type,
		int Toolnumber,
		double val) DUMMY_BODY_RINT;


	// ---- mcToolSetDataExDbl ----
	M4IAPI int MACH_ENTRY mcToolSetDataExDbl(
		MINSTANCE mInst,
		int toolNum,
		const char *fieldName,
		double value) DUMMY_BODY_RINT;


	// ---- mcToolSetDataExInt ----
	M4IAPI int MACH_ENTRY mcToolSetDataExInt(
		MINSTANCE mInst,
		int toolNum,
		const char *fieldName,
		int value) DUMMY_BODY_RINT;


	// ---- mcToolSetDataExStr ----
	M4IAPI int MACH_ENTRY mcToolSetDataExStr(
		MINSTANCE mInst,
		int toolNum,
		const char *fieldName,
		const char *value) DUMMY_BODY_RINT;


	// ---- mcToolSetDesc ----
	M4IAPI int MACH_ENTRY mcToolSetDesc(
		MINSTANCE mInst,
		int toolnum,
		const char *tdsc) DUMMY_BODY_RINT;


	// ---- mcToolSetPropertyDbl ----
	/* mcToolSetPropertyDbl */

	// ---- mcToolSetPropertyInt ----
	/* mcToolSetPropertyInt */

	// ---- mcToolSetPropertyStr ----
	/* mcToolSetPropertyStr */

#ifdef __cplusplus
}
#endif
