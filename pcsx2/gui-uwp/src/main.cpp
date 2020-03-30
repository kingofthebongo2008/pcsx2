#include "pch.h"
#include <memory>
#include <ppltasks.h>


using namespace winrt::Windows::ApplicationModel;
using namespace winrt::Windows::ApplicationModel::Core;
using namespace winrt::Windows::ApplicationModel::Activation;
using namespace winrt::Windows::UI::Core;
using namespace winrt::Windows::UI::ViewManagement;
using namespace winrt::Windows::Graphics::Display;

class ViewProvider : public winrt::implements<ViewProvider, IFrameworkView, IFrameworkViewSource>
{

public:

    ViewProvider() : 
    m_windowClosed(false),
    m_windowVisible(true)
    {
        
    }

    IFrameworkView CreateView()
    {
        return *this;
    }

    // IFrameworkView methods
    virtual void Initialize( const CoreApplicationView& applicationView)
    {
        m_activated     = applicationView.Activated(winrt::auto_revoke, { this, &ViewProvider::OnActivated });
        m_suspending    = CoreApplication::Suspending(winrt::auto_revoke, { this, &ViewProvider::OnSuspending } );
        m_resuming      = CoreApplication::Resuming(winrt::auto_revoke, { this, &ViewProvider::OnResuming });


#if defined(_DEBUG)

        ApplicationView::PreferredLaunchViewSize(winrt::Windows::Foundation::Size(1600, 900));
        ApplicationView::PreferredLaunchWindowingMode(ApplicationViewWindowingMode::PreferredLaunchViewSize);
#else

        //ApplicationView::PreferredLaunchViewSize = Size(1600, 900);
        //ApplicationView::PreferredLaunchWindowingMode(ApplicationViewWindowingMode::FullScreen);

        ApplicationView::PreferredLaunchViewSize(winrt::Windows::Foundation::Size(1600, 900));
        ApplicationView::PreferredLaunchWindowingMode(ApplicationViewWindowingMode::PreferredLaunchViewSize);
#endif

    }

    virtual void Uninitialize()
    {

    }

    virtual void SetWindow( const CoreWindow& window)
    {
        //todo: more events;

        m_size_changed          = window.SizeChanged( winrt::auto_revoke, { this, &ViewProvider::OnWindowSizeChanged });
        m_visibility_changed    = window.VisibilityChanged(winrt::auto_revoke, { this, &ViewProvider::OnVisibilityChanged });
        m_closed                = window.Closed(winrt::auto_revoke, { this, &ViewProvider::OnWindowClosed });

        DisplayInformation currentDisplayInformation = DisplayInformation::GetForCurrentView();

        m_dpi_changed           = currentDisplayInformation.DpiChanged(winrt::auto_revoke, { this, &ViewProvider::OnDpiChanged });
        m_orientation_changed   = currentDisplayInformation.OrientationChanged(winrt::auto_revoke, { this, &ViewProvider::OnOrientationChanged });
        m_display_contents_invalidated = currentDisplayInformation.DisplayContentsInvalidated(winrt::auto_revoke, { this, &ViewProvider::OnDisplayContentsInvalidated });

        m_core_window           = window;
        m_display_information   = currentDisplayInformation;
    }

    virtual void Load( winrt::hstring)
    {

    }

    virtual void Run()
    {
        //show splash screen here
        while (!m_windowClosed)
        {
            CoreWindow::GetForCurrentThread().Dispatcher().ProcessEvents(CoreProcessEventsOption::ProcessAllIfPresent);
        }

        if (m_windowClosed)
        {
            return;
        }

        while (!m_windowClosed)
        {
            if (m_windowVisible)
            {
                CoreWindow::GetForCurrentThread().Dispatcher().ProcessEvents(CoreProcessEventsOption::ProcessAllIfPresent);
            }
            else
            {
                CoreWindow::GetForCurrentThread().Dispatcher().ProcessEvents(CoreProcessEventsOption::ProcessOneAndAllPending);
            }
        }
    }

protected:
    // Event handlers
    void OnActivated(const CoreApplicationView&, const IActivatedEventArgs&)
    {
        CoreWindow::GetForCurrentThread().Activate();
    }

    void OnSuspending( const winrt::Windows::Foundation::IInspectable&, const SuspendingEventArgs& args)
    {
        auto deferral = args.SuspendingOperation().GetDeferral();

        Concurrency::create_task([this, deferral]()
        {
            deferral.Complete();
        });
    }

    void OnResuming(const winrt::Windows::Foundation::IInspectable&, const winrt::Windows::Foundation::IInspectable&)
    {

    }

    void OnWindowSizeChanged( const CoreWindow&, const WindowSizeChangedEventArgs&)
    {
    }

    void OnWindowClosed(const CoreWindow&, const CoreWindowEventArgs&)
    {
        m_windowClosed = true;
    }

    void OnVisibilityChanged(const CoreWindow& , const VisibilityChangedEventArgs& args)
    {
        m_windowVisible = args.Visible();

        if (!m_windowVisible)
        {
            //free render targets here
        }
    }

    // DisplayInformation event handlers.
    void OnDpiChanged(const DisplayInformation& sender, const winrt::Windows::Foundation::IInspectable&)
    {
        m_display_information = sender;

        //attach events again
        m_dpi_changed                   = m_display_information.DpiChanged(winrt::auto_revoke, { this, &ViewProvider::OnDpiChanged });
        m_orientation_changed           = m_display_information.OrientationChanged(winrt::auto_revoke, { this, &ViewProvider::OnOrientationChanged });
        m_display_contents_invalidated  = m_display_information.DisplayContentsInvalidated(winrt::auto_revoke, { this, &ViewProvider::OnDisplayContentsInvalidated });
    }

    void OnOrientationChanged(const DisplayInformation& sender, const winrt::Windows::Foundation::IInspectable&)
    {
        m_display_information = sender;

        //attach events again
        m_dpi_changed = m_display_information.DpiChanged(winrt::auto_revoke, { this, &ViewProvider::OnDpiChanged });
        m_orientation_changed = m_display_information.OrientationChanged(winrt::auto_revoke, { this, &ViewProvider::OnOrientationChanged });
        m_display_contents_invalidated = m_display_information.DisplayContentsInvalidated(winrt::auto_revoke, { this, &ViewProvider::OnDisplayContentsInvalidated });
    }

    void OnDisplayContentsInvalidated(const DisplayInformation& sender, const winrt::Windows::Foundation::IInspectable&)
    {
        m_display_information = sender;

        //attach events again
        m_dpi_changed = m_display_information.DpiChanged(winrt::auto_revoke, { this, &ViewProvider::OnDpiChanged });
        m_orientation_changed = m_display_information.OrientationChanged(winrt::auto_revoke, { this, &ViewProvider::OnOrientationChanged });
        m_display_contents_invalidated = m_display_information.DisplayContentsInvalidated(winrt::auto_revoke, { this, &ViewProvider::OnDisplayContentsInvalidated });

    }

private:

    bool m_windowClosed;
    bool m_windowVisible;

    CoreApplicationView::Activated_revoker                  m_activated;
    CoreApplication::Suspending_revoker                     m_suspending;
    CoreApplication::Resuming_revoker                       m_resuming;

    CoreWindow::SizeChanged_revoker                         m_size_changed;
    CoreWindow::VisibilityChanged_revoker                   m_visibility_changed;
    CoreWindow::Closed_revoker                              m_closed;

    DisplayInformation::DpiChanged_revoker                  m_dpi_changed;
    DisplayInformation::OrientationChanged_revoker          m_orientation_changed;
    DisplayInformation::DisplayContentsInvalidated_revoker  m_display_contents_invalidated;
   
    CoreWindow                                              m_core_window           = nullptr;
    DisplayInformation                                      m_display_information   = nullptr;
};


#pragma warning( disable:4447 )
int32_t __stdcall wWinMain(HINSTANCE, HINSTANCE, PWSTR, int32_t)
{
    //auto viewProviderFactory = ref new ViewProviderFactory();
    CoreApplication::Run(ViewProvider());
    return 0;
}




