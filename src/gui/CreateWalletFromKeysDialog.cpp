// Copyright (c) 2011-2016 The Cryptonote developers
// Copyright (c) 2015-2016 XDN developers
// Copyright (c) 2016-2018 The Karbowanec developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <QApplication>
#include <QFileDialog>
#include <QStandardPaths>
#include "CreateWalletFromKeysDialog.h"
#include "ui_createwalletfromkeysdialog.h"

namespace WalletGui {

CreateWalletFromKeysDialog::CreateWalletFromKeysDialog(QWidget* _parent) : QDialog(_parent), m_ui(new Ui::CreateWalletFromKeysDialog) {
  m_ui->setupUi(this);
  m_ui->m_errorLabel->setText("");
}

CreateWalletFromKeysDialog::~CreateWalletFromKeysDialog() {
}

QString CreateWalletFromKeysDialog::getSpendSecretKeyString() const {
  return m_ui->m_spendPrivateKey->text().trimmed();
}

QString CreateWalletFromKeysDialog::getViewSecretKeyString() const {
  return m_ui->m_viewPrivateKey->text().trimmed();
}

QString CreateWalletFromKeysDialog::getPasswordString() const {
  return m_ui->m_password->text().trimmed();
}

QString CreateWalletFromKeysDialog::getPasswordConfirmString() const {
  return m_ui->m_passwordConfirm->text().trimmed();
}

QString CreateWalletFromKeysDialog::getFilePath() const {
  
  QString defaultDirectory = QDir::currentPath();

  #ifdef Q_OS_MACOS
    defaultDirectory = QDir::homePath();
  #endif
  
  QString walletName = m_ui->m_walletName->text().trimmed();
  return QDir(defaultDirectory).filePath(walletName);
}

void CreateWalletFromKeysDialog::checkPassword(const QString& _password) {

  bool passwordIsConfirmed = false;

  if (!m_ui->m_password->text().trimmed().isEmpty() && !m_ui->m_passwordConfirm->text().trimmed().isEmpty())
  {
    passwordIsConfirmed = !m_ui->m_password->text().compare(m_ui->m_passwordConfirm->text());
    m_ui->m_errorLabel->setText(passwordIsConfirmed ? "" : tr("Password not match"));
  }
  else
  {
    m_ui->m_errorLabel->setText("");
  }

  m_ui->m_okButton->setEnabled(passwordIsConfirmed);
}

// void CreateWalletFromKeysDialog::selectPathClicked() {
  // QString filePath = QFileDialog::getSaveFileName(this, tr("Wallet file"),
// #ifdef Q_OS_WIN
    // //QApplication::applicationDirPath(),
      // QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation),
// #else
    // QDir::homePath(),
// #endif
    // tr("Wallets (*.wallet)")
    // );

  // if (!filePath.isEmpty() && !filePath.endsWith(".wallet")) {
    // filePath.append(".wallet");
  // }

  // m_ui->m_pathEdit->setText(filePath);
// }

// void CreateWalletFromMnemonicSeedDialog::onTextChanged() {
  // wordCount = m_ui->m_seedEdit->toPlainText().split(QRegExp("(\\s|\\n|\\r)+")
                                                  // , QString::SkipEmptyParts).count();
  // if(wordCount != 25) {
    // m_ui->m_okButton->setEnabled(false);
    // m_ui->m_errorLabel->setText(QString::number(wordCount));
    // m_ui->m_errorLabel->setStyleSheet("QLabel { color : red; }");
  // }
  // if(wordCount == 25) {
    // m_ui->m_okButton->setEnabled(true);
    // m_ui->m_errorLabel->setText("OK");
    // m_ui->m_errorLabel->setStyleSheet("QLabel { color : green; }");
  // }
// }

}
