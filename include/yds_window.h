#ifndef YDS_WINDOW_H
#define YDS_WINDOW_H

#include "yds_color.h"
#include "yds_monitor.h"
#include "yds_window_system_object.h"

class ysWindowEventHandler;
class ysWindowSystem;

class ysWindow : public ysWindowSystemObject {
public:
    static const int MAX_NAME_LENGTH = 256;

    enum class WindowState {
        Visible,
        Hidden,
        Maximized,
        Minimized,
        Closed,
        Unknown
    };

    enum class WindowStyle { Windowed, Fullscreen, Popup, Unknown };

public:
    ysWindow();
    ysWindow(Platform platform);
    virtual ~ysWindow();

    /* Interface */
    virtual ysError
    InitializeWindow(ysWindow *parent, const wchar_t *title, WindowStyle style,
                     int x, int y, int width, int height, ysMonitor *monitor,
                     WindowState initialState,
                     const ysVector &color = {0.0f, 0.0f, 0.0f, 1.0f});
    virtual ysError InitializeWindow(ysWindow *parent, const wchar_t *title,
                                     WindowStyle style, ysMonitor *monitor);

    virtual void Close() { SetState(WindowState::Closed); }
    virtual void SetState(WindowState state = WindowState::Visible) {
        m_windowState = state;
    }
    virtual WindowState GetState() const { return m_windowState; }
    virtual void CaptureMouse() {}
    virtual void ReleaseMouse() {}

    void RestoreWindow();

    int GetWidth() const { return m_width; }
    int GetHeight() const { return m_height; }
    virtual int GetScreenWidth() const { return m_width; }
    virtual int GetScreenHeight() const { return m_height; }
    WindowStyle GetCurrentWindowStyle() const { return m_windowStyle; }

    void SetMonitor(ysMonitor *monitor) { m_monitor = monitor; }
    ysMonitor *GetMonitor() const { return m_monitor; }

    const int GetGameWidth() const;
    const int GetGameHeight() const;

    int GetPositionX() const { return m_locationx; }
    int GetPositionY() const { return m_locationy; }

    bool IsOnScreen(int x, int y) const;
    virtual void ScreenToLocal(int &x, int &y) const { /* void */
    }

    bool IsOpen() const;
    virtual bool IsActive();
    virtual bool IsVisible();

    virtual void SetScreenSize(int width, int height);
    virtual void SetWindowSize(int width, int height);
    virtual void SetLocation(int x, int y);
    virtual void SetTitle(const wchar_t *title);

    virtual void StartResizing();
    virtual void EndResizing();
    bool IsResizing() const { return m_resizing; }

    virtual bool SetWindowStyle(WindowStyle style);

    void AttachEventHandler(ysWindowEventHandler *handler);

    void SetGameResolutionScale(float scale);

    void SetGameResolutionScaleHorizontal(float scale);
    float GetGameResolutionScaleHorizontal() const {
        return m_gameResolutionScaleHorizontal;
    }

    void SetGameResolutionScaleVertical(float scale);
    float GetGameResolutionScaleVertical() const {
        return m_gameResolutionScaleVertical;
    }

    inline void SetBackgroundColor(const ysVector &color) {
        m_backgroundColor = color;
    }
    inline ysVector GetBackgroundColor() const { return m_backgroundColor; }

public:
    // Handlers
    void OnMoveWindow(int x, int y);
    void OnResizeWindow(int w, int h);
    void OnActivate();
    void OnDeactivate();
    void OnCloseWindow();
    void OnKeyDown(int key);

protected:
    // Abstraction Layer
    virtual void AL_SetSize(int width, int height);
    virtual void AL_SetLocation(int x, int y);

protected:
    // Window size
    int m_width;
    int m_height;

    // Window location
    int m_locationx;
    int m_locationy;

    // Windowed size
    int m_windowedWidth;
    int m_windowedHeight;

    // Window location
    int m_windowedLocationx;
    int m_windowedLocationy;

    // Title
    wchar_t m_title[MAX_NAME_LENGTH];

    // Current Window State
    WindowState m_windowState;

    // Window Style
    WindowStyle m_windowStyle;

    // Parent Window
    ysWindow *m_parent;

    // The attached monitor for use in fullscreen
    ysMonitor *m_monitor;

    // Window Active
    bool m_active;

    // Status flag indicating whether the window is resizing
    bool m_resizing;

    float m_gameResolutionScaleHorizontal;
    float m_gameResolutionScaleVertical;

    ysVector m_backgroundColor;

protected:
    // Event handler
    ysWindowEventHandler *m_eventHandler;
};

#endif /* YDS_WINDOW_H */
