#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QStandardItemModel>
#include <QMediaPlayer>
#include <QMediaPlaylist>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class QMediaPlayer;

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_file_clicked();

    void on_play_clicked();

    void on_pause_clicked();

    void on_stop_clicked();

    void on_mute_clicked();

    void on_volume_valueChanged(int value);

    void on_rewind_clicked();

    void on_ff_clicked();


private:
    Ui::Widget *ui;
    QMediaPlayer *mMediaPlayer;
    QMediaPlaylist *m_playlist;
    QStandardItemModel  *m_playListModel;
};
#endif // WIDGET_H
