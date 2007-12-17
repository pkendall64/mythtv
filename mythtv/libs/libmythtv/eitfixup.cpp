#include "eitfixup.h"
#include "mythcontext.h"
#include "dvbdescriptors.h" // for MythCategoryType

/*------------------------------------------------------------------------
 * Event Fix Up Scripts - Turned on by entry in dtv_privatetype table
 *------------------------------------------------------------------------*/

EITFixUp::EITFixUp()
    : m_bellYear("[\\(]{1}[0-9]{4}[\\)]{1}"),
      m_bellActors("\\set\\s|,"),
      m_bellPPVTitleAllDay("\\s*\\(All Day.*\\)\\s*$"),
      m_bellPPVTitleHD("^HD\\s?-\\s?"),
      m_bellPPVSubtitleAllDay("^All Day \\(.*\\sEastern\\)$"),
      m_bellPPVDescriptionAllDay("^\\(.*\\sEastern\\)"),
      m_bellPPVDescriptionAllDay2("^\\([0-9].*am-[0-9].*am\\sET\\)"),
      m_bellPPVDescriptionEventId("\\([0-9]{5}\\)"),
      m_ukSubtitle("\\[.*S\\]"),
      m_ukThen("\\s*(Then|Followed by) 60 Seconds\\.", false),
      m_ukNew("\\s*(Brand New|New)\\s*Series\\s*[:\\.\\-]",false),
      m_ukNew1("^New\\."),
      m_ukT4("^[tT]4:"),
      m_ukEQ("[:\\!\\?]"),
      m_ukEPQ("[:\\!\\.\\?]"),
      m_ukColonHyphen("[:-]"),
      m_ukPStart("^\\.+"),
      m_ukPEnd("\\.+$"),
      m_ukSeries1("^\\s*(\\d{1,2})/(\\d{1,2})\\."),
      m_ukSeries2("\\((Part|Pt)\\s+(\\d{1,2})\\s+of\\s+(\\d{1,2})\\)", false),
      m_ukSeries3("\\s*Episode\\s+(\\d{1,2})\\s+of\\s+(\\d{1,2})\\.", false),
      m_ukCC("\\[(AD)(,(S)){,1}(,SL){,1}\\]|\\[(S)(,AD){,1}(,SL){,1}\\]|"
             "\\[(SL)(,AD){,1}(,(S)){,1}\\]"),
      m_ukYear("[\\[\\(]([\\d]{4})[\\)\\]]"),
      m_uk24ep("^\\d{1,2}:00[ap]m to \\d{1,2}:00[ap]m: "),
      m_ukStarring("(?:Western\\s)?[Ss]tarring ([\\w\\s\\-']+)[Aa]nd\\s([\\w\\s\\-']+)[\\.|,](?:\\s)*(\\d{4})?(?:\\.\\s)?"),
      m_ukBBC7rpt("\\[Rptd?[^]]+\\d{1,2}\\.\\d{1,2}[ap]m\\]\\."),
      m_ukCBBC("^CBBC."),
      m_ukCBeebies("^CBeebies."),
      m_ukStarring1("star(ring)"),
      m_ukDoubleDotEnd("\\.\\.+$"),
      m_ukDoubleDotStart("^\\.\\.+"),
      m_comHemCountry("^(\\(.+\\))?\\s?([^ ]+)\\s([^\\.0-9]+)"
                      "(?:\\sfr�n\\s([0-9]{4}))(?:\\smed\\s([^\\.]+))?\\.?"),
      m_comHemDirector("[Rr]egi"),
      m_comHemActor("[Ss]k�despelare|[Ii] rollerna"),
      m_comHemHost("[Pp]rogramledare"),
      m_comHemSub("[.\\?\\!] "),
      m_comHemRerun1("[Rr]epris\\sfr�n\\s([^\\.]+)(?:\\.|$)"),
      m_comHemRerun2("([0-9]+)/([0-9]+)(?:\\s-\\s([0-9]{4}))?"),
      m_comHemTT("[Tt]ext-[Tt][Vv]"),
      m_comHemPersSeparator("(, |\\soch\\s)"),
      m_comHemPersons("\\s?([Rr]egi|[Ss]k�despelare|[Pp]rogramledare|"
                      "[Ii] rollerna):\\s([^\\.]+)\\."),
      m_comHemSubEnd("\\s?\\.\\s?$"),
      m_comHemSeries1("\\s?(?:[dD]el|[eE]pisode)\\s([0-9]+)"
                      "(?:\\s?(?:/|:|av)\\s?([0-9]+))?\\."),
      m_comHemSeries2("\\s?-?\\s?([Dd]el\\s+([0-9]+))"),
      m_comHemTSub("\\s+-\\s+([^\\-]+)"),
      m_mcaIncompleteTitle("(.*).\\.\\.\\.$"),
      m_mcaCompleteTitlea("^'?("),
      m_mcaCompleteTitleb("[^\\.\\?]+[^\\'])'?[\\.\\?]\\s+(.+)"),
      m_mcaSubtitle("^'([^\\.]+)'\\.\\s+(.+)"),
      m_mcaSeries("(^\\d+)\\/(\\d+)\\s-\\s(.*)$"),
      m_mcaCredits("(.*)\\s\\((\\d{4})\\)\\s*([^\\.]+)\\.?\\s*$"),
      m_mcaAvail("\\s(Only available on [^\\.]*bouquet|Not available in RSA [^\\.]*)\\.?"),
      m_mcaActors("(.*\\.)\\s+([^\\.]+\\s[A-Z][^\\.]+)\\.\\s*"),
      m_mcaActorsSeparator("(,\\s+)"),
      m_mcaYear("(.*)\\s\\((\\d{4})\\)\\s*$"),
      m_mcaCC(",?\\s(HI|English) Subtitles\\.?"),
      m_mcaDD(",?\\sDD\\.?"),
      m_RTLrepeat("(\\(|\\s)?Wiederholung.+vo[m|n].+((?:\\d{2}\\.\\d{2}\\.\\d{4})|(?:\\d{2}[:\\.]\\d{2}\\sUhr))\\)?"),
      m_RTLSubtitle1("^Folge\\s(\\d{1,4})\\s*:\\s+'(.*)'(?:\\.\\s*|$)"),
      m_RTLSubtitle2("^Folge\\s(\\d{1,4})\\s+(.{,5}[^\\.]{,120})[\\?!\\.]\\s*"),
      m_RTLSubtitle3("^(?:Folge\\s)?(\\d{1,4}(?:\\/[IVX]+)?)\\s+(.{,5}[^\\.]{,120})[\\?!\\.]\\s*"),
      m_RTLSubtitle4("^Thema.{0,5}:\\s([^\\.]+)\\.\\s*"),
      m_RTLSubtitle5("^'(.+)'\\.\\s*"),
      m_RTLEpisodeNo1("^(Folge\\s\\d{1,4})\\.*\\s*"),
      m_RTLEpisodeNo2("^(\\d{1,2}\\/[IVX]+)\\.*\\s*"),
      m_fiRerun("Uusinta.?"),
      m_Stereo("(Stereo)"),
      m_dePremiereInfos("([^.]+)?\\s?([0-9]{4})\\.\\s[0-9]+\\sMin\\.(?:\\sVon"
                        "\\s([^,]+)(?:,|\\su\\.\\sa\\.)\\smit\\s(.+)\\.)?"),
      m_dePremiereOTitle("\\s*\\(([^\\)]*)\\)$"),
      m_nlStereo("stereo"),
      m_nlTxt("txt"),
      m_nlWide("breedbeeld"),
      m_nlRepeat("herh."),
      m_nlHD("\\sHD$"),
      m_nlSub("\\sAfl\\.:\\s([^\\.]+)\\."),
      m_nlActors("\\sMet:\\s.+e\\.a\\."),
      m_nlPres("\\sPresentatie:\\s([^\\.]+)\\."),
      m_nlPersSeparator("(, |\\sen\\s)"),
      m_nlRub("\\s?\\({1}\\W+\\){1}\\s?"),
      m_nlYear1("(?=\\suit\\s)([1-2]{2}[0-9]{2})"),
      m_nlYear2("([\\s]{1}[\\(]{1}[A-Z]{0,3}/?)([1-2]{2}[0-9]{2})([\\)]{1})"),
      m_nlDirector("(?=\\svan\\s)(([A-Z]{1}[a-z]+\\s)|([A-Z]{1}\\.\\s))"),
      m_nlCat("^(Amusement|Muziek|Informatief|Nieuws/actualiteiten|Jeugd|Animatie|Sport|Serie/soap|Kunst/Cultuur|Documentaire|Film|Natuur|Erotiek|Comedy|Misdaad|Religieus)\\.\\s"),
      m_nlOmroep ("\\s\\(([A-Z]+/?)+\\)$")

{
}

void EITFixUp::Fix(DBEvent &event) const
{
    if (event.fixup)
    {
        if (event.subtitle == event.title)
            event.subtitle = QString::null;

        if (event.description.isEmpty() && !event.subtitle.isEmpty())
        {
            event.description = event.subtitle;
            event.subtitle = QString::null;
        }
    }

    if (kFixHDTV & event.fixup)
        event.flags |= DBEvent::kHDTV;

    if (kFixBell & event.fixup)
        FixBellExpressVu(event);

    if (kFixUK & event.fixup)
        FixUK(event);

    if (kFixPBS & event.fixup)
        FixPBS(event);

    if (kFixComHem & event.fixup)
        FixComHem(event, kFixSubtitle & event.fixup);

    if (kFixAUStar & event.fixup)
        FixAUStar(event);

    if (kFixMCA & event.fixup)
        FixMCA(event);

    if (kFixRTL & event.fixup)
        FixRTL(event);

    if (kFixFI & event.fixup)
        FixFI(event);

    if (kFixPremiere & event.fixup)
        FixPremiere(event);
        
    if (kFixNL & event.fixup)
        FixNL(event);

    if (event.fixup)
    {
        if (!event.title.isEmpty())
            event.title = event.title.stripWhiteSpace();
        if (!event.subtitle.isEmpty())
            event.subtitle = event.subtitle.stripWhiteSpace();
        if (!event.description.isEmpty())
            event.description = event.description.stripWhiteSpace();
    }
}

/**
 *  \brief Use this for the Canadian BellExpressVu to standardize DVB-S guide.
 *  \todo  deal with events that don't have eventype at the begining?
 *  \TODO
 */
void EITFixUp::FixBellExpressVu(DBEvent &event) const
{
    QString tmp = "";

    // A 0x0D character is present between the content
    // and the subtitle if its present
    int position = event.description.find(0x0D);

    if (position != -1)
    {
        // Subtitle present in the title, so get
        // it and adjust the description
        event.subtitle = event.description.left(position);
        event.description = event.description.right(
            event.description.length() - position - 2);
    }

    // Take out the content description which is
    // always next with a period after it
    position = event.description.find(".");
    // Make sure they didn't leave it out and
    // you come up with an odd category
    if (position < 10)
    {
        const QString stmp       = event.description;
        event.description        = stmp.right(stmp.length() - position - 2);
        event.category = stmp.left(position);
    }
    else
    {
        event.category = "Unknown";
    }

    // When a channel is off air the category is "-"
    // so leave the category as blank
    if (event.category == "-")
        event.category = "OffAir";

    if (event.category.length() > 10)
        event.category = "Unknown";


    // See if a year is present as (xxxx)
    position = event.description.find(m_bellYear);
    if (position != -1 && event.category != "")
    {
        tmp = "";
        // Parse out the year
        bool ok;
        uint y = event.description.mid(position + 1, 4).toUInt(&ok);
        if (ok)
            event.originalairdate = QDate(y, 1, 1);
            
        // Get the actors if they exist
        if (position > 3)
        {
            tmp = event.description.left(position-3);
            QStringList actors = QStringList::split(m_bellActors, tmp);
            QStringList::const_iterator it = actors.begin();
            for (; it != actors.end(); it++)
                event.AddPerson(DBPerson::kActor, *it);
        }
        // Remove the year and actors from the description
        event.description = event.description.right(
            event.description.length() - position - 7);
    }

    // Check for (CC) in the decription and
    // set the <subtitles type="teletext"> flag
    position = event.description.find("(CC)");
    if (position != -1)
    {
        event.flags |= DBEvent::kCaptioned;
        event.description = event.description.replace("(CC)", "");
    }

    // Check for (Stereo) in the decription and set the <audio> tags
    position = event.description.find("(Stereo)");
    if (position != -1)
    {
        event.flags |= DBEvent::kStereo;
        event.description = event.description.replace("(Stereo)", "");
    }

    // Check for "title (All Day)" in the title
    position = event.title.find(m_bellPPVTitleAllDay);
    if (position != -1)
    {
        event.title = event.title.replace(m_bellPPVTitleAllDay, "");
    }

    // Check for "HD - title" in the title
    position = event.title.find(m_bellPPVTitleHD);
    if (position != -1)
    {
        event.title = event.title.replace(m_bellPPVTitleHD, "");
        event.flags |= DBEvent::kHDTV;
    }

    // Check for subtitle "All Day (... Eastern)" in the subtitle
    position = event.subtitle.find(m_bellPPVSubtitleAllDay);
    if (position != -1)
    {
        event.subtitle = event.subtitle.replace(m_bellPPVSubtitleAllDay, "");
    }

    // Check for description "(... Eastern)" in the description
    position = event.description.find(m_bellPPVDescriptionAllDay);
    if (position != -1)
    {
        event.description = event.description.replace(m_bellPPVDescriptionAllDay, "");
    }

    // Check for description "(... ET)" in the description
    position = event.description.find(m_bellPPVDescriptionAllDay2);
    if (position != -1)
    {
        event.description = event.description.replace(m_bellPPVDescriptionAllDay2, "");
    }

    // Check for description "(nnnnn)" in the description
    position = event.description.find(m_bellPPVDescriptionEventId);
    if (position != -1)
    {
        event.description = event.description.replace(m_bellPPVDescriptionEventId, "");
    }

}

void EITFixUp::SetUKSubtitle(DBEvent &event) const
{
    QStringList strList1;
    int position1;
    int position2;
    if ((position1 = event.description.find(m_ukEQ)) != -1)
    {
        position2 = event.description.find(":",position1+1);
        if (position2 != -1)
        {
             strList1 = QStringList::split(" ",
                            event.description.mid(position1+1,
                            position2-position1+1));
             if (strList1.count() < kDotToTitle)
             {
                 event.subtitle = event.description.left(position2);
                 event.description = event.description.mid(position2 + 1);
             }
             else
             {
                 event.subtitle = event.description.left(position1);
                 event.description = event.description.mid(position1 + 1);
             }
        }
        else
        {
            if ((uint)position1 < SUBTITLE_MAX_LEN)
            {
                event.subtitle = event.description.left(position1);
                event.description = event.description.mid(position1+1);
            }
        }
    }
}

/** \fn EITFixUp::FixUK(DBEvent&) const
 *  \brief Use this in the United Kingdom to standardize DVB-T guide.
 */
void EITFixUp::FixUK(DBEvent &event) const
{
    int position1;
    int position2;
    QString strFull;

    position1 = event.description.find(m_ukSubtitle);
    if (position1 != -1)
    {
        event.flags |= DBEvent::kSubtitled;
        event.description.remove(m_ukSubtitle);
    }

    // BBC three case (could add another record here ?)
    event.description = event.description.remove(m_ukThen);
    event.description = event.description.remove(m_ukNew);

    event.description = event.description.remove(m_ukNew1);
    event.title  = event.title.remove(m_ukT4);

    // BBC 7 [Rpt of ...] case.
    event.description = event.description.remove(m_ukBBC7rpt);

    QRegExp tmp24ep = m_uk24ep;
    QStringList strList1;
    if (!event.title.startsWith("CSI:") && !event.title.startsWith("CD:"))
    {
        if (((position1=event.title.find(m_ukDoubleDotEnd))!=-1) &&
            ((position2=event.description.find(m_ukDoubleDotStart))!=-1))
        {
            strFull = event.title.remove(m_ukDoubleDotEnd)+" "+
                  event.description.remove(m_ukDoubleDotStart);
            if ((position1 = strFull.find(m_ukEPQ)) != -1)
            {
                 if (strFull[position1] == '!' || strFull[position1] == '?' ||
                     strFull[position1] == '.')
                     position1++;
                 event.title = strFull.left(position1);
                 event.description = strFull.mid(position1 + 1);
                 SetUKSubtitle(event);
            }
            if ((position1 = strFull.find(m_ukYear)) != -1)
            {
                // Looks like they are using the airdate as a delimiter
                if ((uint)position1 < SUBTITLE_MAX_LEN)
                {
                    event.description = event.title.mid(position1);
                    event.title = event.title.left(position1);
                }
            }
        }
        else if ((position1 = tmp24ep.search(event.description)) != -1)
        {
            // Special case for episodes of 24.
            // -2 from the length cause we don't want ": " on the end
            event.subtitle = event.description.mid(position1,
                                tmp24ep.cap(0).length() - 2);
            event.description = event.description.remove(tmp24ep.cap(0));
        }
        else if (((position1 = event.title.find(m_ukColonHyphen)) != -1) &&
            (event.description.find(":") == -1))
        {
            if ((uint)position1 < SUBTITLE_MAX_LEN)
            {
                event.subtitle = event.title.mid(position1 + 1);
                event.title = event.title.left(position1);
            }
        }
        else
            SetUKSubtitle(event);
    }

    QRegExp tmpStarring = m_ukStarring;
    if (tmpStarring.search(event.subtitle) != -1)
    {
        // If the "Starring..." string got promoted to subtitle move it back.
        event.description.prepend(". ");
        event.description.prepend(tmpStarring.cap(0));
        event.subtitle.remove(tmpStarring.cap(0));
    }
    tmpStarring = m_ukStarring;
    if (tmpStarring.search(event.description) != -1)
    {
        // if we match this we've captured 2 actors and an (optional) airdate
        event.AddPerson(DBPerson::kActor, tmpStarring.cap(1));
        event.AddPerson(DBPerson::kActor, tmpStarring.cap(2));
        if (tmpStarring.cap(3).length() > 0)
        {
            event.airdate = tmpStarring.cap(3);
            bool ok;
            uint y = tmpStarring.cap(3).toUInt(&ok);
            if (ok)
                event.originalairdate = QDate(y, 1, 1);
        }
    }

    // Work out the episode numbers (if any)
    bool    series  = false;
    QRegExp tmpExp1 = m_ukSeries1;
    QRegExp tmpExp2 = m_ukSeries2;
    QRegExp tmpExp3 = m_ukSeries3;
    if ((position1 = tmpExp1.search(event.title)) != -1)
    {
        event.partnumber = tmpExp1.cap(1).toUInt();
        event.parttotal  = tmpExp1.cap(2).toUInt();
        // Remove from the title
        event.title =
            event.title.mid(position1 + tmpExp1.cap(0).length());
        series = true;
    }
    else if ((position1 = tmpExp1.search(event.subtitle)) != -1)
    {
        event.partnumber = tmpExp1.cap(1).toUInt();
        event.parttotal  = tmpExp1.cap(2).toUInt();
        // Remove from the sub title
        event.subtitle =
            event.subtitle.mid(position1 + tmpExp1.cap(0).length());
        series = true;
    }
    else if ((position1 = tmpExp1.search(event.description)) != -1)
    {
        event.partnumber = tmpExp1.cap(1).toUInt();
        event.parttotal  = tmpExp1.cap(2).toUInt();
        event.description = event.description.left(position1) +
            event.description.mid(position1 + tmpExp1.cap(0).length());
        series = true;
    }
    else if ((position1 = tmpExp2.search(event.description)) != -1)
    {
        event.partnumber = tmpExp2.cap(2).toUInt();
        event.parttotal  = tmpExp2.cap(3).toUInt();
        event.description = event.description.left(position1) +
            event.description.mid(position1 + tmpExp2.cap(0).length());
        series = true;
    }
    else if ((position1 = tmpExp3.search(event.description)) != -1)
    {
        event.partnumber = tmpExp3.cap(1).toUInt();
        event.parttotal  = tmpExp3.cap(2).toUInt();
        event.description = event.description.left(position1) +
            event.description.mid(position1 + tmpExp3.cap(0).length());
        series = true;
    }
    if (series)
        event.category_type = kCategorySeries;

    // Work out the closed captions and Audio descriptions  (if any)
    QStringList captures;
    QStringList::const_iterator it;
    QRegExp tmpUKCC = m_ukCC;
    if ((position1 = tmpUKCC.search(event.description)) != -1)
    {
        // Enumerate throught and see if we have subtitles, don't modify
        // the description as we might destroy other useful information
        captures = tmpUKCC.capturedTexts();
        for (it = captures.begin(); it != captures.end(); ++it)
        {
            if (*it == "S")
                event.flags |= DBEvent::kSubtitled;
        }
    }
    else if ((position1 = tmpUKCC.search(event.subtitle)) != -1)
    {
        captures = tmpUKCC.capturedTexts();
        for (it = captures.begin(); it != captures.end(); ++it)
        {
            if (*it == "S")
                event.flags |= DBEvent::kSubtitled;
        }

        // We remove [AD,S] from the subtitle.
        QString stmp = event.subtitle;
        int     itmp = position1 + tmpUKCC.cap(0).length();
        event.subtitle = stmp.left(position1) + stmp.mid(itmp);
    }

    // Work out the year (if any)
    QRegExp tmpUKYear = m_ukYear;
    if ((position1 = tmpUKYear.search(event.description)) != -1)
    {
        QString stmp = event.description;
        int     itmp = position1 + tmpUKYear.cap(0).length();
        event.description = stmp.left(position1) + stmp.mid(itmp);
        event.airdate = tmpUKYear.cap(1);
        bool ok;
        uint y = tmpUKYear.cap(1).toUInt(&ok);
        if (ok)
            event.originalairdate = QDate(y, 1, 1);
    }

    // Remove spurious channel names
    event.subtitle = event.subtitle.remove(m_ukCBBC);
    event.description = event.description.remove(m_ukCBBC);
    event.subtitle = event.subtitle.remove(m_ukCBeebies);
    event.description = event.description.remove(m_ukCBeebies);

    // Trim trailing '.'
    event.title.remove(m_ukPEnd);
    event.subtitle.remove(m_ukPEnd);

    if (event.subtitle.isEmpty() && !event.category.startsWith("Movie",false))
    {
       strList1 = QStringList::split(".",event.description);
       QStringList strList2 = QStringList::split("?",event.description);
       if (strList1.count()>1 || strList2.count()>1)
       {
           QString strEnd;
           if (strList1[0].length() > strList2[0].length())
           {
               strList1=strList2;
               strEnd="?";
           }
           strList2 = QStringList::split(" ",strList1[0]);
           if (strList2.count() < kDotToTitle)
           {
               QStringList strList3 = strList2.grep("Drama",false);
               QStringList strList4 = strList2.grep("sitcom",false);
               QStringList strList5 = strList2.grep(m_ukStarring1);
               if ((strList3.count()==0) && (strList4.count()==0) &&
                   (strList5.count()==0))
               {
                    event.subtitle = strList1[0]+strEnd;
                    event.description=
                                 event.description.mid(strList1[0].length()+1);
               }
           }
       }
    }
}

/** \fn EITFixUp::FixPBS(DBEvent&) const
 *  \brief Use this to standardize PBS ATSC guide in the USA.
 */
void EITFixUp::FixPBS(DBEvent &event) const
{
    /* Used for PBS ATSC Subtitles are seperated by a colon */
    int position = event.description.find(':');
    if (position != -1)
    {
        const QString stmp   = event.description;
        event.subtitle = stmp.left(position);
        event.description    = stmp.right(stmp.length() - position - 2);
    }
}

/**
 *  \brief Use this to standardize ComHem DVB-C service in Sweden.
 */
void EITFixUp::FixComHem(DBEvent &event, bool process_subtitle) const
{
    // Reverse what EITFixUp::Fix() did
    if (event.subtitle.isEmpty() && !event.description.isEmpty())
    {
        event.subtitle = event.description;
        event.description = "";
    }

    // Remove subtitle, it contains the category and we already know that
    event.subtitle = "";

    bool isSeries = false;
    // Try to find episode numbers
    int pos;
    QRegExp tmpSeries1 = m_comHemSeries1;
    QRegExp tmpSeries2 = m_comHemSeries2;
    if ((pos = tmpSeries2.search(event.title)) != -1)
    {
        QStringList list = tmpSeries2.capturedTexts();
        event.partnumber = list[2].toUInt();
        event.title = event.title.replace(list[0],"");
    }
    else if ((pos = tmpSeries1.search(event.description)) != -1)
    {
        QStringList list = tmpSeries1.capturedTexts();
        if (!list[1].isEmpty())
        {
            event.partnumber = list[1].toUInt();
        }
        if (!list[2].isEmpty())
        {
            event.parttotal = list[2].toUInt();
        }

        // Remove the episode numbers, but only if it's not at the begining
        // of the description (subtitle code might use it)
        if(pos > 0)
            event.description = event.description.replace(list[0],"");
        isSeries = true;
    }

    // Add partnumber/parttotal to subtitle
    // This will be overwritten if we find a better subtitle
    if (event.partnumber > 0)
    {
        event.subtitle = QString("Del %1").arg(event.partnumber);
        if (event.parttotal > 0)
        {
            event.subtitle += QString(" av %1").arg(event.parttotal);
        }
    }

    // Move subtitle info from title to subtitle
    QRegExp tmpTSub = m_comHemTSub;
    if (tmpTSub.search(event.title) != -1)
    {
        event.subtitle = tmpTSub.cap(1);
        event.title = event.title.replace(tmpTSub.cap(0),"");
    }

    // No need to continue without a description.
    if (event.description.length() <= 0)
        return;

    // Try to find country category, year and possibly other information
    // from the begining of the description
    QRegExp tmpCountry = m_comHemCountry;
    pos = tmpCountry.search(event.description);
    if (pos != -1)
    {
        QStringList list = tmpCountry.capturedTexts();
        QString replacement;

        // Original title, usually english title
        // note: list[1] contains extra () around the text that needs removing
        if (list[1].length() > 0)
        {
            replacement = list[1] + " ";
            //store it somewhere?
        }

        // Countr(y|ies)
        if (list[2].length() > 0)
        {
            replacement += list[2] + " ";
            //store it somewhere?
        }

        // Category
        if (list[3].length() > 0)
        {
            replacement += list[3] + ".";
            if(event.category.isEmpty())
            {
                event.category = list[3];
            }

            if(list[3].find("serie")!=-1)
            {
                isSeries = true;
            }
        }

        // Year
        if (list[4].length() > 0)
        {
            event.airdate = list[4].stripWhiteSpace();
        }

        // Actors
        if (list[5].length() > 0)
        {
            QStringList actors;
            actors=QStringList::split(m_comHemPersSeparator,list[5]);
            for(QStringList::size_type i=0;i<actors.count();i++)
            {
                event.AddPerson(DBPerson::kActor, actors[i]);
            }
        }

        // Remove year and actors.
        // The reason category is left in the description is because otherwise
        // the country would look wierd like "Amerikansk. Rest of description."
        event.description = event.description.replace(list[0],replacement);
    }

    if (isSeries)
        event.category_type = kCategorySeries;

    // Look for additional persons in the description
    QRegExp tmpPersons = m_comHemPersons;
    while(pos = tmpPersons.search(event.description),pos!=-1)
    {
        DBPerson::Role role;
        QStringList list = tmpPersons.capturedTexts();
        
        QRegExp tmpDirector = m_comHemDirector;
        QRegExp tmpActor = m_comHemActor;
        QRegExp tmpHost = m_comHemHost;
        if (tmpDirector.search(list[1])!=-1)
        {
            role = DBPerson::kDirector;
        }
        else if(tmpActor.search(list[1])!=-1)
        {
            role = DBPerson::kActor;
        }
        else if(tmpHost.search(list[1])!=-1)
        {
            role = DBPerson::kHost;
        }
        else
        {
            event.description=event.description.replace(list[0],"");
            continue;
        }

        QStringList actors;
        actors = QStringList::split(m_comHemPersSeparator, list[2]);
        for(QStringList::size_type i=0;i<actors.count();i++)
        {
            event.AddPerson(role, actors[i]);
        }

        // Remove it
        event.description=event.description.replace(list[0],"");
    }

    // Is this event on a channel we shoud look for a subtitle?
    // The subtitle is the first sentence in the description, but the
    // subtitle can't be the only thing in the description and it must be
    // shorter than 55 characters or we risk picking up the wrong thing.
    if (process_subtitle)
    {
        int pos = event.description.find(m_comHemSub);
        bool pvalid = pos != -1 && pos <= 55;
        if (pvalid && (event.description.length() - (pos + 2)) > 0)
        {
            event.subtitle = event.description.left(
                pos + (event.description[pos] == '?' ? 1 : 0));
            event.description = event.description.mid(pos + 2);
        }
    }

    // Teletext subtitles?
    int position = event.description.find(m_comHemTT);
    if (position != -1)
    {
        event.flags |= DBEvent::kSubtitled;
    }

    // Try to findout if this is a rerun and if so the date.
    QRegExp tmpRerun1 = m_comHemRerun1;
    if (tmpRerun1.search(event.description) == -1)
        return;

    // Rerun from today
    QStringList list = tmpRerun1.capturedTexts();
    if (list[1] == "i dag")
    {
        event.originalairdate = event.starttime.date();
        return;
    }

    // Rerun from yesterday afternoon
    if (list[1] == "eftermiddagen")
    {
        event.originalairdate = event.starttime.date().addDays(-1);
        return;
    }

    // Rerun with day, month and possibly year specified
    QRegExp tmpRerun2 = m_comHemRerun2;
    if (tmpRerun2.search(list[1]) != -1)
    {
        QStringList datelist = tmpRerun2.capturedTexts();
        int day   = datelist[1].toInt();
        int month = datelist[2].toInt();
        int year;

        if (datelist[3].length() > 0)
            year = datelist[3].toInt();
        else
            year = event.starttime.date().year();

        if (day > 0 && month > 0)
        {
            QDate date(event.starttime.date().year(), month, day);
            // it's a rerun so it must be in the past
            if (date > event.starttime.date())
                date = date.addYears(-1);
            event.originalairdate = date;
        }
        return;
    }
}

/** \fn EITFixUp::FixAUStar(DBEvent&) const
 *  \brief Use this to standardize DVB-S guide in Australia.
 */
void EITFixUp::FixAUStar(DBEvent &event) const
{
    event.category = event.subtitle;
    /* Used for DVB-S Subtitles are seperated by a colon */
    int position = event.description.find(':');
    if (position != -1)
    {
        const QString stmp   = event.description;
        event.subtitle       = stmp.left(position);
        event.description    = stmp.right(stmp.length() - position - 2);
    }
}

/** \fn EITFixUp::FixMCA(DBEvent&) const
 *  \brief Use this to standardise the MultiChoice Africa DVB-S guide.
 */
void EITFixUp::FixMCA(DBEvent &event) const
{
    const uint SUBTITLE_PCT     = 60; //% of description to allow subtitle to
    const uint SUBTITLE_MAX_LEN = 128;// max length of subtitle field in db.
    bool       dd               = false;
    int        position;
    QRegExp    tmpExp1;

    // Remove subtitle, it contains category information too specific to use
    event.subtitle = QString::null;

    // No need to continue without a description.
    if (event.description.length() <= 0)
        return;

    // Replace incomplete title if the full one is in the description
    tmpExp1 = m_mcaIncompleteTitle;
    if (tmpExp1.search(event.title) != -1)
    {
        tmpExp1 = QString(m_mcaCompleteTitlea.pattern() + tmpExp1.cap(1) +
                          m_mcaCompleteTitleb.pattern());
        tmpExp1.setCaseSensitive(false);
        if (tmpExp1.search(event.description) != -1)
        {
            event.title       = tmpExp1.cap(1).stripWhiteSpace();
            event.description = tmpExp1.cap(2).stripWhiteSpace();
        }
        tmpExp1.setCaseSensitive(true);
    }

    // Try to find subtitle in description
    tmpExp1 = m_mcaSubtitle;
    if ((position = tmpExp1.search(event.description)) != -1)
    {
        if ((tmpExp1.cap(1).length() < SUBTITLE_MAX_LEN) &&
            ((tmpExp1.cap(1).length()*100)/event.description.length() <
             SUBTITLE_PCT))
        {
            event.subtitle    = tmpExp1.cap(1);
            event.description = tmpExp1.cap(2);
        }
    }

    // Try to find episode numbers in subtitle
    tmpExp1 = m_mcaSeries;
    if ((position = tmpExp1.search(event.subtitle)) != -1)
    {
        uint season    = tmpExp1.cap(1).toUInt();
        uint episode   = tmpExp1.cap(2).toUInt();
        event.subtitle = tmpExp1.cap(3).stripWhiteSpace();
        event.syndicatedepisodenumber =
                QString("E%1S%2").arg(episode).arg(season);
        event.category_type = kCategorySeries;
    }

    // Close captioned?
    position = event.description.find(m_mcaCC);
    if (position > 0)
    {
        event.flags |= DBEvent::kCaptioned;
        event.description.replace(m_mcaCC, "");
    }

    // Dolby Digital 5.1?
    position = event.description.find(m_mcaDD);
    if ((position > 0) && (position > (int) (event.description.length() - 7)))
    {
        event.flags |= DBEvent::kStereo;
        dd = true;
        event.description.replace(m_mcaDD, "");
    }

    // Remove bouquet tags
    event.description.replace(m_mcaAvail, "");

    // Try to find year and director from the end of the description
    bool isMovie = false;
    tmpExp1  = m_mcaCredits;
    position = tmpExp1.search(event.description);
    if (position != -1)
    {
        isMovie = true;
        event.description = tmpExp1.cap(1).stripWhiteSpace();
        event.airdate     = tmpExp1.cap(2).stripWhiteSpace();
        event.AddPerson(DBPerson::kDirector, tmpExp1.cap(3).stripWhiteSpace());
    }
    else
    {
        // Try to find year only from the end of the description
        tmpExp1  = m_mcaYear;
        position = tmpExp1.search(event.description);
        if (position != -1)
        {
            isMovie = true;
            event.description = tmpExp1.cap(1).stripWhiteSpace();
            event.airdate     = tmpExp1.cap(2).stripWhiteSpace();
        }
    }

    if (isMovie)
    {
        tmpExp1  = m_mcaActors;
        position = tmpExp1.search(event.description);
        if (position != -1)
        {
            QStringList actors;
            actors = QStringList::split(m_mcaActorsSeparator,tmpExp1.cap(2));
            for (QStringList::size_type i = 0; i < actors.count(); ++i)
                event.AddPerson(DBPerson::kActor, actors[i].stripWhiteSpace());
            event.description = tmpExp1.cap(1).stripWhiteSpace();
        }
        event.category_type = kCategoryMovie;
    }

    //Add descriptive DD tag
    if (dd)
        event.description += " [Dolby 5.1]";
}

/** \fn EITFixUp::FixRTL(DBEvent&) const
 *  \brief Use this to standardise the RTL group guide in Germany.
 */
void EITFixUp::FixRTL(DBEvent &event) const
{
    int        pos;

    // No need to continue without a description and with an subtitle.
    if (event.description.length() <= 0 || event.subtitle.length() > 0)
        return;

    // Repeat
    QRegExp tmpExpRepeat = m_RTLrepeat;
    if ((pos = tmpExpRepeat.search(event.description)) != -1)
    {
        // remove '.' if it matches at the beginning of the description
        int length = tmpExpRepeat.cap(0).length() + (pos ? 0 : 1);
        event.description =
            event.description.remove(pos, length).stripWhiteSpace();
        event.originalairdate = event.starttime.addDays(-1).date();
    }
    
    QRegExp tmpExpSubtitle1 = m_RTLSubtitle1;
    tmpExpSubtitle1.setMinimal(true);
    QRegExp tmpExpSubtitle2 = m_RTLSubtitle2;
    QRegExp tmpExpSubtitle3 = m_RTLSubtitle3;
    QRegExp tmpExpSubtitle4 = m_RTLSubtitle4;
    QRegExp tmpExpSubtitle5 = m_RTLSubtitle5;
    tmpExpSubtitle5.setMinimal(true);
    QRegExp tmpExpEpisodeNo1 = m_RTLEpisodeNo1;
    QRegExp tmpExpEpisodeNo2 = m_RTLEpisodeNo2;

    // subtitle with episode number: "Folge *: 'subtitle'. description
    if (tmpExpSubtitle1.search(event.description) != -1)
    {
        event.syndicatedepisodenumber = tmpExpSubtitle1.cap(1);
        event.subtitle    = tmpExpSubtitle1.cap(2);
        event.description =
            event.description.remove(0, tmpExpSubtitle1.matchedLength());
    }
    // episode number subtitle 
    else if (tmpExpSubtitle2.search(event.description) != -1)
    {
        event.syndicatedepisodenumber = tmpExpSubtitle2.cap(1);
        event.subtitle    = tmpExpSubtitle2.cap(2);
        event.description =
            event.description.remove(0, tmpExpSubtitle2.matchedLength());
    }
    // episode number subtitle 
    else if (tmpExpSubtitle3.search(event.description) != -1)
    {
        event.syndicatedepisodenumber = tmpExpSubtitle3.cap(1);
        event.subtitle    = tmpExpSubtitle3.cap(2);
        event.description =
            event.description.remove(0, tmpExpSubtitle3.matchedLength());
    }
    // "Thema..."
    else if (tmpExpSubtitle4.search(event.description) != -1)
    {
        event.subtitle    = tmpExpSubtitle4.cap(1);
        event.description =
            event.description.remove(0, tmpExpSubtitle4.matchedLength());
    }
    // "'...'"
    else if (tmpExpSubtitle5.search(event.description) != -1)
    {
        event.subtitle    = tmpExpSubtitle5.cap(1);
        event.description =
            event.description.remove(0, tmpExpSubtitle5.matchedLength());
    }
    // episode number
    else if (tmpExpEpisodeNo1.search(event.description) != -1)
    {
        event.syndicatedepisodenumber = tmpExpEpisodeNo1.cap(2);
        event.subtitle    = tmpExpEpisodeNo1.cap(1);
        event.description =
            event.description.remove(0, tmpExpEpisodeNo1.matchedLength());
    }
    // episode number
    else if (tmpExpEpisodeNo2.search(event.description) != -1)
    {
        event.syndicatedepisodenumber = tmpExpEpisodeNo2.cap(2);
        event.subtitle    = tmpExpEpisodeNo2.cap(1);
        event.description =
            event.description.remove(0, tmpExpEpisodeNo2.matchedLength());
    }
}

/** \fn EITFixUp::FixFI(DBEvent&) const
 *  \brief Use this to clean DVB-T guide in Finland.
 */
void EITFixUp::FixFI(DBEvent &event) const
{
    int position = event.description.find(m_fiRerun);
    if (position != -1)
    {
        event.description = event.description.replace(m_fiRerun, "");
    }

    // Check for (Stereo) in the decription and set the <audio> tags
    position = event.description.find(m_Stereo);
    if (position != -1)
    {
        event.flags |= DBEvent::kStereo;
        event.description = event.description.replace(m_Stereo, "");
    }
}

/** \fn EITFixUp::FixPremiere(DBEvent&) const
 *  \brief Use this to standardize DVB-C guide in Germany
 *         for the providers Kabel Deutschland and Premiere.
 */
void EITFixUp::FixPremiere(DBEvent &event) const
{
    QString country = "";

    // Find infos about country and year, regisseur and actors
    QRegExp tmpInfos =  m_dePremiereInfos;
    if (tmpInfos.search(event.description) != -1)
    {
        country = tmpInfos.cap(1).stripWhiteSpace();
        event.airdate = tmpInfos.cap(2);
        event.AddPerson(DBPerson::kDirector, tmpInfos.cap(3));
        QStringList actors = QStringList::split(", ", tmpInfos.cap(4));
        for(QStringList::size_type j=0;j<actors.count();j++)
            event.AddPerson(DBPerson::kActor, actors[j]);
        event.description = event.description.replace(tmpInfos.cap(0), "");
    }

    // move the original titel from the title to subtitle
    QRegExp tmpOTitle = m_dePremiereOTitle;
    if (tmpOTitle.search(event.title) != -1)
    {
        event.subtitle = QString("%1, %2").arg(tmpOTitle.cap(1)).arg(country);
        event.title = event.title.replace(tmpOTitle.cap(0), "");
    }
}

/** \fn EITFixUp::FixNL(DBEvent&) const
 *  \brief Use this to standardize \@Home DVB-C guide in the Netherlands.
 */
void EITFixUp::FixNL(DBEvent &event) const
{
    QString fullinfo = "";
    fullinfo.append (event.subtitle);
    fullinfo.append (event.description);
    event.subtitle = "";

    // Convert categories to Dutch categories Myth knows.
    // nog invoegen: comedy, sport, misdaad

    if (event.category == "Documentary")
    {
        event.category = "Documentaire";
        event.category_type = kCategoryNone;
    }
    if (event.category == "News")
    {
        event.category = "Nieuws/actualiteiten";
        event.category_type = kCategoryNone;
    }
    if (event.category == "Kids")
    {
        event.category = "Jeugd";
        event.category_type = kCategoryNone;
    }
    if (event.category == "Show/game Show")
    {
        event.category = "Amusement";
        event.category_type = kCategoryTVShow;
    }
    if (event.category == "Music/Ballet/Dance")
    {
        event.category = "Muziek";
        event.category_type = kCategoryNone;
    }
    if (event.category == "News magazine")
    {
        event.category = "Informatief";
        event.category_type = kCategoryNone;
    }
    if (event.category == "Movie")
    {
        event.category = "Film";
        event.category_type = kCategoryMovie;
    }
    if (event.category == "Nature/animals/Environment")
    {
        event.category = "Natuur";
        event.category_type = kCategoryNone;
    }
    if (event.category == "Movie - Adult")
    {
        event.category = "Erotiek";
        event.category_type = kCategoryNone;
    }
    if (event.category == "Movie - Soap/melodrama/folkloric")
    {
        event.category = "Serie/soap";
        event.category_type = kCategorySeries;
    }
    if (event.category == "Arts/Culture")
    {
        event.category = "Kunst/Cultuur";
        event.category_type = kCategoryNone;
    }
    if (event.category == "Sports")
    {
        event.category = "Sport";
        event.category_type = kCategorySports;
    }
    if (event.category == "Cartoons/Puppets")
    {
        event.category = "Animatie";
        event.category_type = kCategoryNone;
    }
    if (event.category == "Movie - Comedy")
    {
        event.category = "Comedy";
        event.category_type = kCategorySeries;
    }
    if (event.category == "Movie - Detective/Thriller")
    {
        event.category = "Misdaad";
        event.category_type = kCategoryNone;
    }
    if (event.category == "Social/Spiritual Sciences")
    {
        event.category = "Religieus";
        event.category_type = kCategoryNone;
    }

    // Get stereo info
    int position;
    if ((position = fullinfo.find(m_nlStereo)) != -1)
    {
        event.flags |= DBEvent::kStereo;
        fullinfo = fullinfo.replace("stereo", ".");
    }
    
    //Get widescreen info
    if ((position = fullinfo.find(m_nlWide)) != -1)
    {
        fullinfo = fullinfo.replace("breedbeeld", ".");
    }

    // Get repeat info
    if ((position = fullinfo.find(m_nlRepeat)) != -1)
    {
        fullinfo = fullinfo.replace("herh.", ".");
    }

    // Get teletext subtitle info
    if ((position = fullinfo.find(m_nlTxt)) != -1)
    {
        event.flags |= DBEvent::kSubtitled;
        fullinfo = fullinfo.replace("txt", ".");
    }
    
    // Get HDTV information
    if ((position = event.title.find(m_nlHD)) != -1)
    {
        event.flags |= DBEvent::kHDTV;
        event.title = event.title.replace(m_nlHD, "");
    }

    // Try to make subtitle
    QRegExp tmpSub = m_nlSub;
    QString tmpSubString;
    if (tmpSub.search(fullinfo) != -1)
    {
        tmpSubString = tmpSub.cap(0);
        tmpSubString = tmpSubString.right(tmpSubString.length() - 7);
        event.subtitle = tmpSubString.left(tmpSubString.length() -1);
        fullinfo = fullinfo.replace(tmpSub.cap(0), "");
    }
    
    // This is trying to catch the case where the subtitle is in the main title
    // but avoid cases where it isn't a subtitle e.g cd:uk
    if (((position = event.title.find(":")) != -1) &&
        (event.title[position + 1].upper() == event.title[position + 1]) &&
        (event.subtitle.isEmpty()))
    {
        event.subtitle = event.title.mid(position + 1);
        event.title = event.title.left(position);
    }

    
    // Get the actors
    QRegExp tmpActors = m_nlActors;
    QStringList actors;
    QString tmpActorsString;
    if (tmpActors.search(fullinfo) != -1)
    {
        tmpActorsString = tmpActors.cap(0);
        tmpActorsString = tmpActorsString.right(tmpActorsString.length() - 6);
        tmpActorsString = tmpActorsString.left(tmpActorsString.length() - 5);
        actors = QStringList::split(QString(", "), tmpActorsString);
        for (QStringList::size_type i =0; i < actors.count(); i++)
        {
            event.AddPerson(DBPerson::kActor, actors[i]);
        }
        fullinfo = fullinfo.replace(tmpActors.cap(0), "");
    }

    // Try to find presenter
    QRegExp tmpPres = m_nlPres;
    QStringList host;
    QString tmpPresString;
    if (tmpPres.search(fullinfo) != -1)
    {
        tmpPresString = tmpPres.cap(0);
        tmpPresString = tmpPresString.right(tmpPresString.length() - 14);
        tmpPresString = tmpPresString.left(tmpPresString.length() -1);
        host = QStringList::split(m_nlPersSeparator, tmpPresString);
        for (QStringList::size_type i =0; i < host.count(); i++)
        {
            event.AddPerson(DBPerson::kPresenter, host[i]);
        }
        fullinfo = fullinfo.replace(tmpPres.cap(0), "");
    }

    // Try to find year
    QRegExp tmpYear1 = m_nlYear1;
    QRegExp tmpYear2 = m_nlYear2;
    if ((position = tmpYear1.search(fullinfo)) != -1)
    {
        bool ok;
        uint y = tmpYear1.cap(0).toUInt(&ok);
        if (ok)
            event.originalairdate = QDate(y, 1, 1);
    }
    
    if ((position = tmpYear2.search(fullinfo)) != -1)
    {
        bool ok;
        uint y = tmpYear2.cap(2).toUInt(&ok);
        if (ok)
            event.originalairdate = QDate(y, 1, 1);
    }

    // Try to find director
    QRegExp tmpDirector = m_nlDirector;
    QString tmpDirectorString;
    if ((position = fullinfo.find(m_nlDirector)) != -1)
    {
        tmpDirectorString = tmpDirector.cap(0);
        event.AddPerson(DBPerson::kDirector, tmpDirectorString);
    }

    // Strip leftovers
    if ((position = fullinfo.find(m_nlRub)) != -1)
    {
        fullinfo = fullinfo.replace(m_nlRub, "");
    }
    
    // Strip category info from description
    if ((position = fullinfo.find(m_nlCat)) != -1)
    {
        fullinfo = fullinfo.replace(m_nlCat, "");
    }
    
    // Remove omroep from title
    if ((position = event.title.find(m_nlOmroep)) != -1)
    {
        event.title = event.title.replace(m_nlOmroep, "");
    }

    // Put information back in description

    event.description = fullinfo;
    event.description.stripWhiteSpace();
    event.title.stripWhiteSpace();
    event.subtitle.stripWhiteSpace();

}
