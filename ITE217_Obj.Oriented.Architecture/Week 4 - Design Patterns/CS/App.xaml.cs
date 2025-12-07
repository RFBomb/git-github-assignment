//
// Robert Brenckman
// Charter Oak - ITE217 ( Fall 2025 )
// Week 4 - Singletons
// 11/22/2025
//

using Microsoft.Extensions.DependencyInjection;
using System.Windows;

namespace ITE217_Week4_SingletonPattern
{
    /// <summary>
    /// Interaction logic for App.xaml
    /// </summary>
    public partial class App : Application
    {
        private static IServiceProvider? serviceProvider;

        public static IServiceProvider ServiceProvider => serviceProvider!; // set during app startup, should never GC

        protected override void OnStartup(StartupEventArgs e)
        {
            var serviceCollection = new Microsoft.Extensions.DependencyInjection.ServiceCollection();
            serviceCollection.AddSingleton<MvvmDialogs.IDialogService>((p) => new MvvmDialogs.DialogService());

            serviceProvider = serviceCollection.BuildServiceProvider();

            base.OnStartup(e);
        }

        protected override void OnActivated(EventArgs e)
        {
            if (MainWindow.DataContext is null)
            {
                MainWindow.DataContext = ActivatorUtilities.CreateInstance<ViewModels.MainWindowViewModel>(ServiceProvider);
            }
            base.OnActivated(e);
        }
    }
}
