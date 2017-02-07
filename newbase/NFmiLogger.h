// ======================================================================
/*!
 * \file NFmiLogger.h
 * \brief Interface of class NFmiLogger
 */
// ======================================================================

#ifndef NFMILOGGER_H
#define NFMILOGGER_H

#include "NFmiMetTime.h"
#include <string>

//! Undocumented
class _FMI_DLL NFmiLogger
{
 public:
  //! loggerissa k‰ytettyj‰ viestien tasoja
  enum Level
  {
    kNoLevel = 0,
    kImportantInfo = 1,
    kInfo = 2,
    kDebugInfo = 4,
    kMinorWarning = 8,
    kWarning = 16,
    kError = 32,
    kFatalError = 64
  };

  //! loggerissa k‰ytettyj‰ laitteita
  enum Device
  {
    kNoDevice = 0,
    kFile = 1,
    kGsm = 2,
    kEmail = 4,
    kStdOut = 8,
    kStdErr = 16
  };

  //! loggerissa k‰ytettyj‰ tiedosto backup systeemin tiedoston vaihto taajuus
  enum Period
  {
    kNoPeriod = 0,
    kHourly =
        1,  // lokitiedostosta tehd‰‰n tallenne (renametaan log-file) aina kun tunti on vaihtunut
    kDaily =
        2,  // lokitiedostosta tehd‰‰n tallenne (renametaan log-file) aina kun p‰iv‰ on vaihtunut
    kMonthly =
        4  // lokitiedostosta tehd‰‰n tallenne (renametaan log-file) aina kun kuukausi on vaihtunut
  };

  ~NFmiLogger(void);
  NFmiLogger(void);
  NFmiLogger(const std::string &theLogFilePath,
             const std::string &theLogFileBaseName,
             bool theUseBackupSystem,
             bool theCleanBackupFiles,
             Period theLoggingPeriod);
  void InitFromSettings(const std::string &theBaseNameSpace);
  void StoreToSettings(void);
  //  bool Init(const std::string & theFileName);
  //  bool Store(const std::string & theFileName);
  bool LogMessage(const std::string &theMessage, Level theMessageLevel);
  bool ForceLogMessage(const std::string &theMessage, Level theMessageLevel, Device theDevice);
  void AddLevel(Level theAddedLevel);
  void RemoveLevel(Level theRemovedLevel);
  void UsedLoggingLevels(int value);
  int UsedLoggingLevels(void) const;
  void LogFilePath(const std::string &value);
  const std::string &LogFilePath(void) const;
  void LogFileBaseName(const std::string &value);
  const std::string &LogFileBaseName(void) const;
  void Operational(bool value);
  bool Operational(void) const;
  bool UseLocalTime(void) const;
  void UseLocalTime(bool newValue);
  void LogFileBackupTimePeriod(Period value);
  Period LogFileBackupTimePeriod(void) const;
  void UsedLoggingDevices(int value);
  int UsedLoggingDevices(void) const;
  const std::string &TimeStampStringFormat(void) const;
  void TimeStampStringFormat(const std::string &newFormat);
  bool IsLoggerInitialized(void) const { return fLoggerInitialized; }
  const std::string &CurrentLogFileName(void) const { return itsCurrentLogFileName; }
  bool LogJustMessages(void) const { return fLogJustMessages; }
  void LogJustMessages(bool newValue) { fLogJustMessages = newValue; }
  bool NewLogFileInUse(void) { return fNewLogFileInUse; }
  void NewLogFileInUse(bool newState) { fNewLogFileInUse = newState; }
 private:
  void UpdateFileNamesAndPaths(
      bool updatePath);  // t‰m‰ p‰ivitt‰‰ itsAbsolutLogFilePath:in ja itsCurrentLogFileName:n
  void MakeCurrentLogFileName(void);
  const std::string MakeBackupFileFilter(void);
  bool CheckBackupSystem(
      void);  // tekee kaiken mahdollisen liittyen mahdollisiin backup systeemeihin
  bool CleanBackupFiles(void);
  bool LogMessage(const std::string &theMessage, Level theMessageLevel, int theDevices);
  std::string MakeFinalMessage(const std::string &theMessage, Level theMessageLevel);
  bool Log2Gsm(const std::string &theFinalMessage);
  bool Log2File(const std::string &theFinalMessage);
  bool Log2Email(const std::string &theFinalMessage);
  bool Log2StdOut(const std::string &theFinalMessage);
  bool Log2StdErr(const std::string &theFinalMessage);
  bool OpenFile(const std::string &theFullFileName,
                const std::string &thePath,
                std::ofstream &theFile);
  bool IsFileBackUpTime(void);
  bool IsCleaningTime(void);
  bool MakeBackupFile(void);
  bool DirectoryExist(const std::string &thePath);
  bool TooOldBackupFileTimeStamp(std::time_t theFileTimeStamp);

  void GetLastLoggetTimeFromLogFile(
      void);  // kun logger k‰ynnistyy, katsotaan loki-tiedostosta, milloin on viimeksi tehty lokia
  const std::string MakeBackupFileNameTimeStamp(void);

  // T‰h‰n voidaan biteill‰ tallettaa eritasoiset k‰ytett‰v‰t logattavat viestitasot ks. enum Level.
  // esim. kImportantInfo + kError (2 + 64 = 66)
  int itsUsedLoggingLevels;
  std::string itsLogFilePath;
  std::string itsAbsolutLogFilePath;  // t‰t‰ k‰ytet‰‰n sitten sis‰isesti, eik‰ olla tyˆhakemiston
                                      // muutosten armoilla
  std::string itsLogFileBaseName;
  std::string itsTimeStampStringFormat;  // t‰h‰n tulee tulevaisuudessa hieno formatointi, mutta
                                         // t‰ss‰ vaiheessa sis‰lt‰‰ luvun jolla saadaan
                                         // time-luokalta haluttu time-string esim. 120 =
                                         // YYYYMMDDHHMMSS
  int itsTimeStampStringIntegerFormat;   // t‰h‰n lasketaan toistaiseksi ed. format juttu valmiiksi
                                         // kun viel‰ k‰ytet‰‰n simppeli‰ integer lukua osoittamaan
                                         // formaatti
  // T‰m‰ kertoo kuika usein on tarkoitus tehd‰ log-file backupeja. Tunnin, p‰iv‰n vai kuukauden
  // v‰lein (ks. enum Period)
  Period itsLogFileBackupTimePeriod;
  NFmiTime itsCurrentTime;  // t‰st‰ otetaan aikaleima ja mahd. vertailut backup systeemiss‰
                            // edelliseen aikaan
  // T‰m‰n avulla on tarkoitus katsoa, pit‰‰kˆ luoda nykyisest‰ log-filesta backup-file (ja mink‰
  // niminen).
  NFmiTime itsLastLoggedTime;
  // Sanomia voidaa tallettaa moniin laitteisiin. T‰h‰n on laitetta bittein‰ talteen, mit‰ laitteita
  // k‰ytet‰‰n (ks. enum Device).
  // esim. kFile (=2), jolloin viestit menev‰t vain tiedostoon
  int itsUsedLoggingDevices;
  // Loggeri voidaan laittaa pois k‰ytˆst‰ jos halutaan.
  bool fOperational;
  bool fUseLocalTime;     // k‰ytet‰‰nkˆ kaikissa hommissa lokaali aikaa, vai UTC-aikaa?
  bool fUseBackupSystem;  // Tekeekˆ luokka myˆs backup loki-tiedostoja (aikaleimoilla varustettuja)
  bool fCleanBackupFiles;   // siivoaako luokka myˆs backup loki-tiedostoja
  bool fLoggerInitialized;  // t‰ll‰ voidaan tarkistaa onko loggeri jo ainakin kerran initialisoitu
  bool fLogJustMessages;    // t‰m‰ on j‰lkeen lis‰tty moodi joka on defaulttina pois p‰‰lt‰ ja se
                            // pit‰‰ aina laittaa erikseen p‰‰lle (ei konffissa ainakaan viel‰).
  // Jos siis t‰m‰ on p‰‰ll‰, ei laiteta loki-viestiin mit‰‰n aika leimoja, pellk‰ annettu viesti.
  bool fNewLogFileInUse;  // S‰‰detyin v‰liajoin k‰ytetty lokitiedosto laitetaan talteen omalla
                          // nimell‰‰n ja luodaan uusi
  // tiedosto lokitusta varten. Kun n‰in tapahtuu, laitetaan t‰m‰ lippu p‰‰lle.
  int itsKeepFilesDays;  // kuinka monta p‰iv‰‰ talletetaan tiedostoja (jos siivotaan)
  // Riitt‰‰ kun t‰m‰ lasketaan kerran, ja sitten sit‰ voidaan k‰ytt‰‰ jatkossa suoraan. Tai jos
  // loki tiedoston nimest‰ tulee muuttuva
  // (esim. aikaleima), t‰h‰n koostetaan se.
  std::string itsCurrentLogFileName;

  // n‰m‰ eri tasoisten sanomien labelit kertovat , mita talletettavaan vietiin tulee esim.
  // 20011003153412: ErrorLabel: Itse viesti tulee t‰h‰n.
  std::string itsNoLevelLabel;
  std::string itsImportantInfoLabel;
  std::string itsInfoLabel;
  std::string itsDebugInfoLabel;
  std::string itsMinorWarningLabel;
  std::string itsWarningLabel;
  std::string itsErrorLabel;
  std::string itsFatalErrorLabel;

  std::string itsBaseNameSpace;
};  // class NFMiLogger

// ----------------------------------------------------------------------
/*!
 *†\param value Undocumented
 */
// ----------------------------------------------------------------------

inline void NFmiLogger::UsedLoggingLevels(int value) { itsUsedLoggingLevels = value; }
// ----------------------------------------------------------------------
/*!
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline int NFmiLogger::UsedLoggingLevels(void) const { return itsUsedLoggingLevels; }
// ----------------------------------------------------------------------
/*!
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline const std::string &NFmiLogger::LogFilePath(void) const { return itsLogFilePath; }
// ----------------------------------------------------------------------
/*!
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline const std::string &NFmiLogger::LogFileBaseName(void) const { return itsLogFileBaseName; }
// ----------------------------------------------------------------------
/*!
 *†\param value Undocumented
 */
// ----------------------------------------------------------------------

inline void NFmiLogger::Operational(bool value) { fOperational = value; }
// ----------------------------------------------------------------------
/*!
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline bool NFmiLogger::Operational(void) const { return fOperational; }
// ----------------------------------------------------------------------
/*!
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline bool NFmiLogger::UseLocalTime(void) const { return fUseLocalTime; }
// ----------------------------------------------------------------------
/*!
 *†\param newValue Undocumented
 */
// ----------------------------------------------------------------------

inline void NFmiLogger::UseLocalTime(bool newValue) { fUseLocalTime = newValue; }
// ----------------------------------------------------------------------
/*!
 *†\param value Undocumented
 */
// ----------------------------------------------------------------------

inline void NFmiLogger::LogFileBackupTimePeriod(Period value)
{
  itsLogFileBackupTimePeriod = value;
}

// ----------------------------------------------------------------------
/*!
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline NFmiLogger::Period NFmiLogger::LogFileBackupTimePeriod(void) const
{
  return itsLogFileBackupTimePeriod;
}

// ----------------------------------------------------------------------
/*!
 *†\param value Undocumented
 */
// ----------------------------------------------------------------------

inline void NFmiLogger::UsedLoggingDevices(int value) { itsUsedLoggingDevices = value; }
// ----------------------------------------------------------------------
/*!
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline int NFmiLogger::UsedLoggingDevices(void) const { return itsUsedLoggingDevices; }
// ----------------------------------------------------------------------
/*!
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline const std::string &NFmiLogger::TimeStampStringFormat(void) const
{
  return itsTimeStampStringFormat;
}

#endif  // NFMILOGGER_H

// ======================================================================
