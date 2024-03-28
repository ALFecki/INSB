function Component() {
    installer.addWizardPage(component, "KeyValidation", QInstaller.ReadyForInstallation);
    component.userInterface("KeyValidation").pushButton.clicked.connect(validateKey);
    component.userInterface("KeyValidation").complete = false;
}

function validateKey() {
    key = component.userInterface("KeyValidation").lineEdit.text
    if (key === "1234-5678-1111-1111") {
        component.userInterface("KeyValidation").complete = true;
    }
}

