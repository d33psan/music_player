#include "widget.h"
#include "ui_widget.h"
#include<QMediaPlayer>
#include<QFileDialog>
#include <QDir>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    m_playListModel = new QStandardItemModel(this);
    ui->playlistView->setModel(m_playListModel);

    m_playListModel->setHorizontalHeaderLabels(QStringList()  << tr("Audio Track")
                                                            << tr("File Path"));
    ui->playlistView->hideColumn(1);
    ui->playlistView->verticalHeader()->setVisible(false);
    ui->playlistView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->playlistView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->playlistView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ui->playlistView->horizontalHeader()->setStretchLastSection(true);

    mMediaPlayer=new QMediaPlayer(this);
    connect(mMediaPlayer,&QMediaPlayer::positionChanged,[&](qint64 pos){
        ui->progressBar->setValue(pos);
    });

    connect(mMediaPlayer, &QMediaPlayer::durationChanged,[&](qint64 dur){
       ui->progressBar->setMaximum(dur);
    });
    m_playlist = new QMediaPlaylist(mMediaPlayer);
    mMediaPlayer->setPlaylist(m_playlist);
    mMediaPlayer->setVolume(25);
    m_playlist->setPlaybackMode(QMediaPlaylist::Loop);

    connect(ui->next, &QPushButton::clicked, m_playlist, &QMediaPlaylist::next);
    connect(ui->previous, &QPushButton::clicked, m_playlist, &QMediaPlaylist::previous);
    connect(ui->play, &QPushButton::clicked, mMediaPlayer, &QMediaPlayer::play);
    connect(ui->pause, &QPushButton::clicked, mMediaPlayer, &QMediaPlayer::pause);
    connect(ui->stop, &QPushButton::clicked, mMediaPlayer, &QMediaPlayer::stop);

    connect(ui->playlistView, &QTableView::doubleClicked, [this](const QModelIndex &index){
        m_playlist->setCurrentIndex(index.row());
    });



    connect(m_playlist, &QMediaPlaylist::currentIndexChanged, [this](int index){
        ui->currentTrack->setText(m_playListModel->data(m_playListModel->index(index, 0)).toString());
    });

}

Widget::~Widget()
{
    delete ui;
    delete m_playListModel;
    delete m_playlist;
    delete mMediaPlayer;
}


void Widget::on_file_clicked()
{
   QStringList files = QFileDialog::getOpenFileNames(this,
                                                     tr("Open files"),
                                                     QString(),
                                                     tr("Audio Files (*.mp3)"));



   foreach (QString filePath, files) {
       QList<QStandardItem *> items;
       items.append(new QStandardItem(QDir(filePath).dirName()));
       items.append(new QStandardItem(filePath));
       m_playListModel->appendRow(items);
       m_playlist->addMedia(QUrl(filePath));
   }
}

void Widget::on_play_clicked()
{
    mMediaPlayer->play();
}

void Widget::on_pause_clicked()
{
    mMediaPlayer->pause();
}

void Widget::on_stop_clicked()
{
    mMediaPlayer->stop();
}



void Widget::on_mute_clicked()
{
    if(ui->mute->text()=="Mute")
    {
    mMediaPlayer->setMuted(true);
    ui->mute->setIcon(QIcon(":/resnew/Resources/icons8-no-audio-100.png"));
    ui->mute->setText("Unmute");
    }
    else {
        mMediaPlayer->setMuted(false);
        ui->mute->setIcon(QIcon(":/resnew/Resources/icons8-audio-100.png"));
        ui->mute->setText("Mute");
    }
}

void Widget::on_volume_valueChanged(int value)
{
    mMediaPlayer->setVolume(value);
}

void Widget::on_rewind_clicked()
{
        mMediaPlayer->setPosition(mMediaPlayer->position()-10000);
}

void Widget::on_ff_clicked()
{
        mMediaPlayer->setPosition(mMediaPlayer->position()+10000);
}

