#pragma once
#include <memory>
#include <vector>
#include "Windows/UIWindowBase.h"

class WindowManager
{
protected:
	friend class Renderer;
	static void Render();
public:

	template<typename T, typename... Args>
	static void Add(Args&&... args) {
		PendingUIWindows.push_back(std::make_unique<T>(std::forward<Args>(args)...));
	}

	// unsafe
	//template<typename T, typename... Args>
	//static void AddImmediate(Args&&... args) {
	//	AddInternal(std::make_unique<T>(std::forward<Args>(args)...));
	//}

	static void FlushPendingWindows();

private:
	static void AddInternal(std::unique_ptr<UIWindowBase> Window);

	static inline std::vector<std::unique_ptr<UIWindowBase>> UIWindows;
	static inline std::vector<std::unique_ptr<UIWindowBase>> PendingUIWindows;	
};
