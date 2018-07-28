#include "trainerbase.h"

class WinToastHandlerExample : public WinToastLib::IWinToastHandler {
public:
	WinToastHandlerExample() {
	};
	// Public interfaces - no override now
	void toastActivated() const override {
	};
	void toastActivated(int actionIndex) const override {
	};
	void toastDismissed(WinToastDismissalReason state) const override {
	};
	void toastFailed() const override {
	};
};


// since we do not minimize often, it's ok to init every time when we need toast.
bool initToast() {
	if (!WinToastLib::WinToast::isCompatible()) {
		logln("Error, toast system version in not supported!");
		return false;
	}
	WinToastLib::WinToast::instance()->setAppName(L"2kToast");
	const auto aumi = WinToastLib::WinToast::configureAUMI(L"yo1995", L"toast", L"toastexample", L"20180726");
	WinToastLib::WinToast::instance()->setAppUserModelId(aumi);
	if (!WinToastLib::WinToast::instance()->initialize()) {
		logln("Error, could not initialize the toast lib!");
		return false;
	}
	WinToastHandlerExample* handler = new WinToastHandlerExample;
	WinToastLib::WinToastTemplate templ = WinToastLib::WinToastTemplate(WinToastLib::WinToastTemplate::Text02);
	templ.setTextField(L"NBA 2K11 is deactivated.", WinToastLib::WinToastTemplate::FirstLine);
	templ.setTextField(L"The trainer is paused.", WinToastLib::WinToastTemplate::SecondLine);

	if (!WinToastLib::WinToast::instance()->showToast(templ, handler)) {
		logln("Error: Could not launch your toast notification!");
		return false;
	}
	return true;
}

