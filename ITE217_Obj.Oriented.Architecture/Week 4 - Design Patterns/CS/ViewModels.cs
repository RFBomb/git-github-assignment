//
// Robert Brenckman
// Charter Oak - ITE217 ( Fall 2025 )
// Week 4 - Singletons
// 11/22/2025
//

using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;
using Microsoft.Extensions.DependencyInjection;
using MvvmDialogs;
using System.ComponentModel;

namespace ITE217_Week4_SingletonPattern.ViewModels
{
    public partial class MainWindowViewModel(IServiceProvider serviceProvider, MvvmDialogs.IDialogService dialogService) : ObservableObject
    {
        private readonly IServiceProvider _serviceProvider = serviceProvider ?? throw new ArgumentNullException(nameof(serviceProvider));
        private readonly MvvmDialogs.IDialogService _dialogService = dialogService ?? throw new ArgumentNullException(nameof(dialogService));
        private readonly HashSet<PopupOneViewModel> _popups = []; 

        ///<summary>
        ///Create a non-modal dialog of <see cref="PopupOneViewModel"/>
        ///</summary> 
        [RelayCommand]
        private void ShowPopupOne()
        {
            var vm = ActivatorUtilities.CreateInstance<PopupOneViewModel>(_serviceProvider, _popups.Count + 1);
            _dialogService.Show(this, vm);
            _popups.Add(vm);
            vm.Closed += Vm_Closed;
        }

        private void Vm_Closed(object? sender, EventArgs e)
        {
            if (sender is PopupOneViewModel vm)
            {
                _popups.Remove(vm);
                vm.Closed -= Vm_Closed;
            }
        }

        ///<summary>
        ///Create a modal dialog of <see cref="PopupOneViewModel"/>
        ///</summary> 
        [RelayCommand]
        private void ShowPopupTwo()
        {
            _dialogService.ShowDialog(this, ActivatorUtilities.CreateInstance<PopupTwoViewModel>(_serviceProvider));
        }
    }

    public partial class PopupOneViewModel(MvvmDialogs.IDialogService dialogService, int instance) : ObservableObject
    {
        private bool _isClosing = false;
        private readonly MvvmDialogs.IDialogService _dialogService = dialogService;

        public string WindowTitle { get; } = $"Non-Modal Instance #{instance}";

        public event EventHandler? Closed;

        [RelayCommand]
        private void Close()
        {
            _isClosing = true;
            _dialogService.Close(this);
            Closed?.Invoke(this, EventArgs.Empty);
        }

        /// <summary>
        /// Handle 'x' button and alt-f4 via code-behind in the window
        /// </summary>
        public void WindowClosing(object? sender, CancelEventArgs e)
        {
            if (_isClosing == false)
            {
                _isClosing = true;
                Closed?.Invoke(this, e);
            }
        }
    }

    public partial class PopupTwoViewModel(MvvmDialogs.IDialogService dialogService) : ObservableObject, IModalDialogViewModel
    {
        private readonly MvvmDialogs.IDialogService _dialogService = dialogService;

        public string Question { get; } = "This is a modal Dialog - operation cannot proceed until you click 'OK'";
        private bool success;

        [RelayCommand]
        private void OK()
        {
            success = true;
            _dialogService.Close(this);
        }

        public bool? DialogResult => success;
    }
}
