#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "openssl_api.h"
#include "myhelper.h"

#include <QTextCursor>




MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //默认勾选 DES  ECB
    ui->radioButton_ecb->setChecked(true);
    ui->radioButton_des->setChecked(true);

    WaringLabel = new QLabel;
    PosLabel = new QLabel;
    SelLabel = new QLabel;

    WaringLabel->setStyleSheet("color:red;font: bold 20px");

    PosLabel->setStyleSheet("font: 18px");
    SelLabel->setStyleSheet("font: 18px");
    PosLabel->setText("Pos:0");
    SelLabel->setText("Sel:0");

    ui->statusBar->addWidget(WaringLabel);       //警告信息 显示在左侧
    ui->statusBar->addPermanentWidget(PosLabel);
    ui->statusBar->addPermanentWidget(SelLabel);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete WaringLabel;
    delete PosLabel;
    delete SelLabel;
}

void MainWindow::on_tabWidget_tabBarClicked(int index)
{
    WaringLabel->setText( "" );
    SelLabel->setText( QString("Sel:%1").arg(0) );
    PosLabel->setText( QString("Pos:%1").arg(0) );
}


//加密
void MainWindow::on_pushButton_enc_clicked()
{
    QString keyHex = ui->lineEdit_key->text();
    if (keyHex.isEmpty()) {
        WaringLabel->setText( tr("key is empty") );
        return;
    }

    QString ivHex = "";
    if( !ui->radioButton_ecb->isChecked() )
    {
        ivHex = ui->lineEdit_iv->text();
        if (ivHex.isEmpty()) {            
            WaringLabel->setText( tr("iv is empty") );
            return;
        }
    }


    QString inHex = ui->textEdit_in->toPlainText();
    if (inHex.isEmpty()) {        
        WaringLabel->setText( tr("input is empty") );
        return;
    }

    //选择加密算法和加密模式
    int alg,mode;
    if( ui->radioButton_des->isChecked() ){
        alg = DES;
    }else if( ui->radioButton_aes->isChecked()){
        alg = AES;
    }else if( ui->radioButton_sm4->isChecked()){
        alg = SM4;
    }
    if( ui->radioButton_ecb->isChecked() ){
        mode = ECB;
    }else if( ui->radioButton_cbc->isChecked()){
        mode = CBC;
    }else if( ui->radioButton_cfb->isChecked()){
        mode = CFB;
    }else if( ui->radioButton_ofb->isChecked()){
        mode = OFB;
    }


    ui->textEdit_out->clear();

    QString outHex;
    int ret = 0;
    ret = OPENSSL_API::enc(keyHex,ivHex,alg,mode,inHex,outHex);
    if(ret<0){
        WaringLabel->setText( tr("enc error") );
        return;
    }

    WaringLabel->setText( tr("") );

    ui->textEdit_out->setText(outHex);
    return;
}

void MainWindow::on_pushButton_dec_clicked()
{
    QString keyHex = ui->lineEdit_key->text();
    if (keyHex.isEmpty()) {
        WaringLabel->setText( tr("key is empty") );
        return;
    }

    QString ivHex = "";
    if( !ui->radioButton_ecb->isChecked() )
    {
        ivHex = ui->lineEdit_iv->text();
        if (ivHex.isEmpty()) {
            WaringLabel->setText( tr("iv  is empty") );
            return;
        }
    }


    QString inHex = ui->textEdit_in->toPlainText();
    if (inHex.isEmpty()) {
        WaringLabel->setText( tr("input is empty") );
        return;
    }

    //选择解密算法和解密模式
    int alg,mode;
    if( ui->radioButton_des->isChecked() ){
        alg = DES;
    }else if( ui->radioButton_aes->isChecked()){
        alg = AES;
    }else if( ui->radioButton_sm4->isChecked()){
        alg = SM4;
    }
    if( ui->radioButton_ecb->isChecked() ){
        mode = ECB;
    }else if( ui->radioButton_cbc->isChecked()){
        mode = CBC;
    }else if( ui->radioButton_cfb->isChecked()){
        mode = CFB;
    }else if( ui->radioButton_ofb->isChecked()){
        mode = OFB;
    }


    ui->textEdit_out->clear();

    QString outHex;
    int ret = 0;
    ret = OPENSSL_API::dec(keyHex,ivHex,alg,mode,inHex,outHex);
    if(ret<0){
        WaringLabel->setText( tr("dec error") );
        return;
    }

    WaringLabel->setText( tr("") );

    ui->textEdit_out->setText(outHex);
    return;

}



void MainWindow::on_textEdit_in_cursorPositionChanged()
{
    int pos = ui->textEdit_in->textCursor().position();
    PosLabel->setText(QString("Pos:%1").arg(pos));
}


void MainWindow::on_textEdit_in_selectionChanged()
{
    QTextCursor cur = ui->textEdit_in->textCursor();
    int len = cur.selectionEnd() - cur.selectionStart();
    SelLabel->setText( QString("Sel:%1").arg(len) );
}


void MainWindow::on_textEdit_out_cursorPositionChanged()
{
    int pos = ui->textEdit_out->textCursor().position();
    PosLabel->setText(QString("Pos:%1").arg(pos));
}

void MainWindow::on_textEdit_out_selectionChanged()
{
    QTextCursor cur = ui->textEdit_out->textCursor();
    int len = cur.selectionEnd() - cur.selectionStart();
    SelLabel->setText( QString("Sel:%1").arg(len) );
}
///////////////////对称算法对话框  end//////////////////////


void MainWindow::on_lineEdit_key_cursorPositionChanged(int arg1, int arg2)
{
    int pos = ui->lineEdit_key->cursorPosition();
    PosLabel->setText(QString("Pos:%1").arg(pos));
}

void MainWindow::on_lineEdit_key_selectionChanged()
{
    int len = ui->lineEdit_key->selectedText().length();
    SelLabel->setText( QString("Sel:%1").arg(len) );
}

void MainWindow::on_lineEdit_iv_cursorPositionChanged(int arg1, int arg2)
{
    int pos = ui->lineEdit_iv->cursorPosition();
    PosLabel->setText(QString("Pos:%1").arg(pos));
}

void MainWindow::on_lineEdit_iv_selectionChanged()
{
    int len = ui->lineEdit_iv->selectedText().length();
    SelLabel->setText( QString("Sel:%1").arg(len) );
}



///////////////////字符处理  ///////////////////////
void MainWindow::on_pushButton_asctohex_clicked()
{
    WaringLabel->setText("");
    QString in = ui->textEdit_char_in->toPlainText();
    if (in.isEmpty() ){
        return;
    }
    //QString  outHex = myHelper::byteArrayToHexStr(in.toUtf8());
    QString  outHex ( in.toUtf8().toHex() );
    outHex.toUpper();
    ui->textEdit_char_out->setText(outHex);
}

void MainWindow::on_pushButton_hextoasc_clicked()
{
    WaringLabel->setText("");
    QString in = ui->textEdit_char_in->toPlainText();
    if (in.isEmpty() ){
        return;
    }
    QByteArray tbyte = QByteArray::fromHex( in.toUtf8() );
    if( tbyte.isEmpty()){
        WaringLabel->setText("data error");
        return ;
    }
    QString outAsc(tbyte);
    ui->textEdit_char_out->setText(outAsc);
}

void MainWindow::on_pushButton_upper_clicked()
{
    WaringLabel->setText("");
    QString in = ui->textEdit_char_in->toPlainText();
    ui->textEdit_char_out->setText(in.toUpper());
}

void MainWindow::on_pushButton_lower_clicked()
{
    WaringLabel->setText("");
    QString in = ui->textEdit_char_in->toPlainText();
    ui->textEdit_char_out->setText(in.toLower());
}

void MainWindow::on_pushButton_xor_clicked()
{
    WaringLabel->setText("");
    QString in1 = ui->textEdit_char_in->toPlainText();
    QString in2 = ui->textEdit_char_in_2->toPlainText();

    QByteArray bytein1 =  QByteArray::fromHex(in1.toUtf8());
    QByteArray bytein2 =  QByteArray::fromHex(in2.toUtf8());

    if( bytein1.isEmpty()){
        WaringLabel->setText("in1 error");
        return ;
    }
    if( bytein2.isEmpty()){
        WaringLabel->setText("in2 error");
        return ;
    }
    if( bytein1.length() != bytein2.length()){
        WaringLabel->setText("data len not equal");
        return ;
    }

    QByteArray result;
    for(int i=0;i<bytein1.length();i++){
        result.append( bytein1.at(i) ^ bytein2.at(i) );
    }
    QString strResult =  result.toHex();
    ui->textEdit_char_out->setText(strResult.toUpper());
}

void MainWindow::on_pushButton_not_clicked()
{
    WaringLabel->setText("");

    QString in1 = ui->textEdit_char_in->toPlainText();
    QByteArray bytein1 =  QByteArray::fromHex(in1.toUtf8());
    if( bytein1.isEmpty()){
        WaringLabel->setText("in1 error");
        return ;
    }
    QByteArray bytein2(bytein1.length() , (char)0xff);

    QByteArray result;
    for(int i=0;i<bytein1.length();i++){
        result.append( bytein1.at(i) ^ bytein2.at(i) );
    }
    QString strResult =  result.toHex();
    ui->textEdit_char_out->setText(strResult.toUpper());
}

void MainWindow::on_pushButton_trim_clicked()
{
    WaringLabel->setText("");
    QString in = ui->textEdit_char_in->toPlainText();

    QString trimStr = ui->lineEdit_trim_str->text();

    QString out ;
    if( trimStr.isEmpty()){

        for(int i =0;i<in.length();i++){
            if(in.at(i).isSpace()){
                in.remove(i,1);
            }
        }
    }
    else{
        in.remove( trimStr,  Qt::CaseSensitive);
    }
    ui->textEdit_char_out->setText(in);
}

void MainWindow::on_pushButton_base64tohex_clicked()
{
    WaringLabel->setText("");
    QString in = ui->textEdit_char_in->toPlainText();
    QByteArray bytet = QByteArray::fromBase64( in.toUtf8() );
    QString strt(bytet.toHex());
    ui->textEdit_char_out->setText(strt.toUpper());
}

void MainWindow::on_pushButton_hextobase64_clicked()
{
    WaringLabel->setText("");
    QString in = ui->textEdit_char_in->toPlainText();
    QByteArray bytet = QByteArray::fromHex( in.toUtf8() );
    QString strt( bytet.toBase64() );
    ui->textEdit_char_out->setText(strt);
}



void MainWindow::on_pushButton_save_clicked()
{
    WaringLabel->setText("");
    QString in = ui->textEdit_char_in->toPlainText();
    QByteArray bytet = QByteArray::fromHex( in.toUtf8() );

    QString path = QFileDialog::getSaveFileName(this,tr("Save File"),  ".",   tr("bin(*.bin);;*(*)"));
    if(!path.isEmpty()) {
        QFile file(path);
        if ( !file.open(QIODevice::ReadWrite) )
        {
            QMessageBox::warning(this, tr("Save File"),  tr("Cannot save file:\n%1").arg(path));
            return;
        }
        if( bytet.length() != file.write(bytet) ){
            QMessageBox::warning(this, tr("Save File"),  tr("Cannot save file:\n%1").arg(path));
        }
        file.close();
    }
    else{
        QMessageBox::warning(this, tr("Save File"),  tr("Cannot save file:\n%1").arg(path));
    }
}

void MainWindow::on_pushButton_load_clicked()
{
    QByteArray bytet;
    QString path = QFileDialog::getOpenFileName(this,tr("Open File"),  ".",   tr("bin(*.bin);;*(*)"));
    if(!path.isEmpty()) {
        QFile file(path);
        if ( !file.open(QIODevice::ReadOnly) )
        {
            QMessageBox::warning(this, tr("Open File"),  tr("Cannot Open file:\n%1").arg(path));
            return;
        }
        bytet = file.readAll();

        file.close();
    }
    else{
        QMessageBox::warning(this, tr("Open File"),  tr("Cannot save file:\n%1").arg(path));
    }

    QByteArray byteHex =  bytet.toHex();
    QString outStr(byteHex);
    ui->textEdit_char_out->setText(outStr.toUpper());
}



void MainWindow::on_textEdit_char_in_cursorPositionChanged()
{
    int pos = ui->textEdit_char_in->textCursor().position();
    PosLabel->setText(QString("Pos:%1").arg(pos));
}

void MainWindow::on_textEdit_char_in_selectionChanged()
{
    QTextCursor cur = ui->textEdit_char_in->textCursor();
    int len = cur.selectionEnd() - cur.selectionStart();
    SelLabel->setText( QString("Sel:%1").arg(len) );
}

void MainWindow::on_textEdit_char_in_2_cursorPositionChanged()
{
    int pos = ui->textEdit_char_in_2->textCursor().position();
    PosLabel->setText(QString("Pos:%1").arg(pos));
}

void MainWindow::on_textEdit_char_in_2_selectionChanged()
{
    QTextCursor cur = ui->textEdit_char_in_2->textCursor();
    int len = cur.selectionEnd() - cur.selectionStart();
    SelLabel->setText( QString("Sel:%1").arg(len) );
}

void MainWindow::on_textEdit_char_out_cursorPositionChanged()
{
    int pos = ui->textEdit_char_out->textCursor().position();
    PosLabel->setText(QString("Pos:%1").arg(pos));
}

void MainWindow::on_textEdit_char_out_selectionChanged()
{
    QTextCursor cur = ui->textEdit_char_out->textCursor();
    int len = cur.selectionEnd() - cur.selectionStart();
    SelLabel->setText( QString("Sel:%1").arg(len) );
}



void MainWindow::on_pushButton_rsa_gen_clicked()
{
    QString bits = ui->lineEdit_bits->text();
    QString e = ui->lineEdit_e->text();

    QString derpk;
    QString dervk;
    OPENSSL_API::genrsa(bits,e,derpk,dervk);
}