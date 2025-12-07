//
// Robert Brenckman
// Charter Oak - ITE217 ( Fall 2025 )
// Week 4 - Singletons
// 11/22/2025
//

using ITE217_Week4_SingletonPattern.ViewModels;
using System.Windows;

namespace ITE217_Week4_SingletonPattern.Views
{
    /// <summary>
    /// Interaction logic for PopupOneView.xaml
    /// </summary>
    public partial class PopupOne : Window
    {
        public PopupOne()
        {
            InitializeComponent();
        }

        private void Window_Closing(object sender, System.ComponentModel.CancelEventArgs e)
        {
            if (DataContext is PopupOneViewModel vm)
            {
                vm.WindowClosing(this, e);
            }
        }
    }
}
