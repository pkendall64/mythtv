#ifndef MUSICCOMMON_H_
#define MUSICCOMMON_H_

// qt
#include <QKeyEvent>
#include <QObject>

// mythtv
#include <audiooutput.h>
#include <mythscreentype.h>

// mythmusic
#include "metadata.h"
#include "playlist.h"
#include "musicplayer.h"
#include "mythactions.h"

class Output;
class Decoder;
class QTimer;
class MythUIProgressBar;
class MythUIImage;
class MythUIText;
class MythUIStateType;
class MythUIButton;
class MythUIVideo;
class MythUIButton;
class MythUICheckBox;
class MythMenu;

enum MusicView
{
    MV_PLAYLIST,
    MV_LYRICS,
    MV_PLAYLISTEDITORTREE,
    MV_PLAYLISTEDITORGALLERY,
    MV_VISUALIZER,
    MV_SEARCH,
    MV_ARTISTINFO,
    MV_ALBUMINFO,
    MV_TRACKINFO,
    MV_RADIO
};

Q_DECLARE_METATYPE(MusicView);

class MPUBLIC MusicCommon : public MythScreenType
{
    Q_OBJECT

  public:

    bool doSelect(const QString &action);
    bool doEscape(const QString &action);
    bool doUp(const QString &action);
    bool doDown(const QString &action);
    bool doThmbUp(const QString &action);
    bool doThmbDown(const QString &action);
    bool doNextTrack(const QString &action);
    bool doPrevTrack(const QString &action);
    bool doFfwd(const QString &action);
    bool doRwnd(const QString &action);
    bool doPause(const QString &action);
    bool doPlay(const QString &action);
    bool doStop(const QString &action);
    bool doCycleVis(const QString &action);
    bool doBlankScr(const QString &action);
    bool doVolumeDown(const QString &action);
    bool doVolumeUp(const QString &action);
    bool doSpeedDown(const QString &action);
    bool doSpeedUp(const QString &action);
    bool doMute(const QString &action);
    bool doToggleUpmix(const QString &action);
    bool doInfo(const QString &action);
    bool doEdit(const QString &action);
    bool doDelete(const QString &action);
    bool doMenu(const QString &action);
    bool doRefresh(const QString &action);
    bool doMark(const QString &action);

  protected:

    MusicCommon(MythScreenStack *parent, const QString &name);
    ~MusicCommon(void);

    bool CreateCommon(void);

    void switchView(MusicView view);

    virtual void customEvent(QEvent *event);
    bool keyPressEvent(QKeyEvent *e);

    virtual void ShowMenu(void);

    //bool onMediaEvent(MythMediaDevice *pDev);

  protected slots:

    void play(void);
    void stop(void);
    void pause(void);
    void previous(void);
    void next(void);
    void seekforward(void);
    void seekback(void);
    void seek(int);
    void stopAll(void);
    void changeRating(bool increase);
//    void editPlaylist(void);
//    void nextAuto(void);

    void searchButtonList(void);
    MythMenu* createMainMenu(void);
    MythMenu* createViewMenu(void);
    MythMenu* createPlaylistMenu(void);
    MythMenu* createPlayerMenu(void);
    MythMenu* createQuickPlaylistsMenu(void);
    MythMenu* createRepeatMenu(void);
    MythMenu* createShuffleMenu(void);
    MythMenu* createVisualizerMenu(void);
    MythMenu* createPlaylistOptionsMenu(void);

    void playlistItemClicked(MythUIButtonListItem *item);
    void playlistItemVisible(MythUIButtonListItem *item);

    void fromCD(void);
    void allTracks(void);
    void byArtist(void);
    void byAlbum(void);
    void byGenre(void);
    void byYear(void);
    void byTitle(void);
    void doUpdatePlaylist(void);

  protected:
    void showExitMenu(void);
    void showPlaylistOptionsMenu(bool addMainMenu = false);

    QString getTimeString(int exTime, int maxTime);
    void updateProgressBar(void);
    void setTrackOnLCD(Metadata *mdata);
    void editTrackInfo(Metadata *mdata);
    void updateTrackInfo(Metadata *mdata);
    void showTrackInfo(Metadata *mdata);
    void updateUIPlaylist(void);
    void updatePlaylistStats(void);
    void updateUIPlayedList(void);    // for streaming
    void updateRepeatMode(void);
    void updateShuffleMode(bool updateUIList = false);

    void changeVolume(bool up);
    void changeSpeed(bool up);
    void toggleMute(void);
    void toggleUpmix(void);
    void showVolume(void);
    void updateVolume(void);
    void showSpeed(bool show);

    void startVisualizer(void);
    void stopVisualizer(void);
    void cycleVisualizer(void);
    void switchVisualizer(const QString &visual);
    void switchVisualizer(int visual);

    void playFirstTrack();
    bool restorePosition(int trackID);

    MusicView              m_currentView;

    // visualiser stuff
    MainVisual            *m_mainvisual;
    bool                   m_fullscreenBlank;
    bool                   m_cycleVisualizer;
    bool                   m_randomVisualizer;

    QStringList            m_visualModes;
    unsigned int           m_currentVisual;

    bool                   m_moveTrackMode;
    bool                   m_movingTrack;

    bool                   m_controlVolume;

    int                    m_currentTrack;
    int                    m_currentTime;
    int                    m_maxTime;

    uint                   m_playlistTrackCount;
    uint                   m_playlistPlayedTime;
    uint                   m_playlistMaxTime;

    // for quick playlists
    PlaylistOptions        m_playlistOptions;
    QString                m_whereClause;

    // for adding tracks from playlist editor
    QList<int>             m_songList;

    // UI widgets
    MythUIText            *m_timeText;
    MythUIText            *m_infoText;
    MythUIText            *m_visualText;
    MythUIText            *m_noTracksText;

    MythUIStateType       *m_shuffleState;
    MythUIStateType       *m_repeatState;

    MythUIStateType       *m_movingTracksState;

    MythUIStateType       *m_ratingState;

    MythUIProgressBar     *m_trackProgress;
    MythUIText            *m_trackProgressText;
    MythUIText            *m_trackSpeedText;
    MythUIStateType       *m_trackState;

    MythUIStateType       *m_muteState;
    MythUIText            *m_volumeText;

    MythUIProgressBar     *m_playlistProgress;

    MythUIButton          *m_prevButton;
    MythUIButton          *m_rewButton;
    MythUIButton          *m_pauseButton;
    MythUIButton          *m_playButton;
    MythUIButton          *m_stopButton;
    MythUIButton          *m_ffButton;
    MythUIButton          *m_nextButton;

    MythUIImage           *m_coverartImage;

    MythUIButtonList      *m_currentPlaylist;
    MythUIButtonList      *m_playedTracksList;

    MythUIVideo           *m_visualizerVideo;

  private:
    MythActions<MusicCommon> *m_actions;
};

class MPUBLIC MythMusicVolumeDialog : public MythScreenType
{
    Q_OBJECT
  public:
    MythMusicVolumeDialog(MythScreenStack *parent, const char *name);
    ~MythMusicVolumeDialog(void);

    bool Create(void);
    bool keyPressEvent(QKeyEvent *event);

    bool doVolumeUp(const QString &action);
    bool doVolumeDown(const QString &action);
    bool doMute(const QString &action);

  protected:
    void increaseVolume(void);
    void decreaseVolume(void);
    void toggleMute(void);

    void updateDisplay(void);

    QTimer            *m_displayTimer;
    MythUIText        *m_messageText;
    MythUIText        *m_volText;
    MythUIStateType   *m_muteState;
    MythUIProgressBar *m_volProgress;

  private:
    MythActions<MythMusicVolumeDialog> *m_actions;
};

class MPUBLIC TrackInfoDialog : public MythScreenType
{
    Q_OBJECT
  public:
    TrackInfoDialog(MythScreenStack *parent, Metadata *mdata, const char *name);
    ~TrackInfoDialog(void);

    bool Create(void);
    bool keyPressEvent(QKeyEvent *event);

    bool doInfo(const QString &action);

  protected:
    Metadata *m_metadata;

  private:
    MythActions<TrackInfoDialog> *m_actions;
};

#endif
