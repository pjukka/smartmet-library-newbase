// ======================================================================
/*!
 * \file NFmiInfoData.h
 * \brief NFmiInfoData namespace declarations \par
 *
 * T‰h‰n on ker‰tty m‰‰rityksi‰, mit‰ tarvitaan mm. erilaisten maskien
 * ja datojen m‰‰rityksiss‰. Olen ker‰nnyt ne t‰h‰n, koska Metkun editorin
 * luokkakirjastojen siivous vaatii tietyn koodin siirt‰mist‰ newbase:en.
 */
// ======================================================================

#ifndef NFMIINFODATA_H
#define NFMIINFODATA_H

namespace NFmiInfoData
{
//! Classification of smartinfo data (Metkun editorissa k‰ytettyjen datojen luokituksia)
enum Type
{
  kNoDataType = 0,    // t‰m‰ on mm. virhetilanteita varten
  kEditable = 1,      // t‰t‰ voi editoida, katsoa ja maskata
  kViewable = 2,      // t‰t‰ dataa ei ole tarkoitus editoida vaan esim. katsoa ja k‰ytt‰‰ maskina
                      // (HUOM! t‰m‰ on myˆs ns. p‰‰ malli datat tyyppi, ks. kModelHelpData)
  kStationary = 3,    // tarkoittaa mm. topografiaa (eli ei muutu ajan mukana)
  kCopyOfEdited = 4,  // alkuper‰isen editoitavan datan kopio, k‰ytet‰‰n mm. vertailtaessa n‰ytˆll‰
                      // alkuper‰ist‰ dataa ja uutta muutettua dataa
  kObservations = 5,  // havaintodataa
  kCalculatedValue = 6,  // t‰m‰ on mm. maskin luokitusta varten eli esim. latitude/longitude maskia
                         // varten tai tms., joka ei ole queryinfosta poimittu
  kKepaData = 7,  // t‰m‰ on viimeisin tietokannassa oleva data (mm. aikasarjoihin vertailun vuoksi)
  kClimatologyData = 8,  // klimatologista dataa (mm. l‰mpˆtilan fraktaalit aikasarjaan apudataksi)
  kAnalyzeData = 9,      // Esim. Mesa:in tuottama analyysi data, jota k‰ytet‰‰n mm. "analyysin
                         // liu'uttamisessa ennusteeseen" -tyˆkalussa
  kScriptVariableData = 10,  // skiptiss‰ olevien muuttujien hanskaaminen tapahtuu t‰m‰n tyyppisen
                             // 'calculaattorin' avulla
  kAnyData = 11,  // t‰m‰ on jokeri-tyyppi, kun on annettu ep‰m‰‰r‰inen parid ja/tai prodid ja pit‰‰
                  // etsi‰ sit‰ l‰pi kaikkien datojen
  kSatelData = 12,              // satelliitti kuva dataa
  kFlashData = 13,              // salama dataa
  kMacroParam = 14,             // makro-parametri eli esim. RESULT = T_EC - T_HIR makrolla laskettu
  kHybridData = 15,             // mallipinta dataa
  kFuzzyData = 16,              // kalman/fuzzy filttereill‰ muokattua dataa
  kVerificationData = 17,       // verifioinnissa k‰ytett‰v‰‰ dataa (analyysi - ennuste vertailua)
  kModelHelpData = 18,          // t‰h‰n laitetaan tietyn mallin ns. apudatat, joissa on esim. apu
                                // parametreja, pika dataa ja dataa eri alueilta
                                // Huom! nyt laitetaan myˆs esi lasketut sounding-index datat
  kTrajectoryHistoryData = 19,  // t‰h‰n laitetaan tietyn mallin ns. trajektorihistoria datat
  kTEMPCodeSoundingData = 20,   // t‰h‰n laitetaan TEMP-koodeista puretut luotausdatat
  kCrossSectionMacroParam =
      21,  // poikkileikkaus makro-parametri eli esim. result = T_EC - T_HIR makrolla laskettu
  kEditingHelpData = 22,  // Kun editori on ns. helpdata-moodissa, sill‰ l‰hetettyj‰ datoja luetaan
                          // sis‰‰n ja k‰ytet‰‰n t‰m‰n data tyyppisin‰
  kConceptualModelData = 23,  // Kun editorilla halutaan katsoa k‰siteanalyysej‰, on t‰m‰ datatyyppi
  kSingleStationRadarData = 24,  // T‰m‰ data tyyppi on sit‰ varten ett‰ jos tulee yksitt‰isille
                                 // asemille tutkadataa (aluksi t‰m‰n avulla tehtiin saudi param
                                 // popup-valikko)
  kQ3MacroParam = 25,  // makro-parametri joka lasketaan Q3-enginell‰ eli esim. RESULT = T_EC -
                       // T_HIR makrolla laskettu
  kSoundingParameterData =
      1000  // HUOM! t‰st‰ on tarkoitus p‰‰st‰ eroon, mukana nyt siksi ett‰ on olemassa
            // n‰yttˆmakroja, joissa on k‰ytetty t‰t‰.
  // Vanha k‰yttˆ: luotaus parametrien (CAPE, CIN, jne.) erikois laskelmat t‰t‰ kautta, laitoin
  // luvuksi 100, koska joudun laskemaan yhteen aina kaksi datatype‰ esim. kHybrid ja t‰m‰n, ett‰
  // tied‰n mink‰ tyyppist‰ dataa k‰yttet‰‰n laskuissa
};

// SmartMetissa on muutama erikoisparamtri, joiden enumin laitoin nyt toistaiseksi t‰h‰n
enum SpecialParameters
{
  kFmiSpSynoPlot = 997,  // synop-plot parametri
  kFmiSpDrawDifferenceParam =
      999,  // kun SmartMetissa piirret‰‰n editoidun datan ja originaali datan eroa, k‰ytet‰‰n t‰t‰
  kFmiSpTvsTdQualityCheckParam =
      9991,  // Smartmetin data-quality check erikois parametri eli T >= Td -tarkistus
  kFmiSpStreamline = 9992,  // TotalWind-parametrille lis‰t‰‰n tietyiss‰ tilanteissa uusi meta-param
                            // streamline piirtoa varten. T‰m‰ tulee siis vain SmartMetin
                            // popup-valikkoihin
  kFmiSpEcmwf3Vrk = 9993,   // Pit‰‰ tehd‰ viritys 3 vrk EC datojen kanssa, ett‰ ne saadaan mukaan
                            // smarttool-kieleen, eli smarttool-intepreter laittaa ec3vrk-tuottajat
                           // t‰ll‰isiksi ja smarttool-modifier muuttaa haut oikeisiin tuottajiin ja
                           // datatyyppeihin
  kFmiSpSelectedGridPoints = 9994,  // kun visualisoidaan editoinnissa valittuja pisteita
                                    // toolmasterilla, k‰ytet‰‰n t‰t‰ parametria
  kFmiSpMetarPlot = 9995,           // synop-plot:iin liittyv‰ metar-plot parametri
  kFmiSpMinMaxPlot = 9996,          // synop-plot:iin liittyv‰ minmax-plot parametri
  kFmiSpSoundingPlot = 9997,        // synop-plot:iin liittyv‰ sounding-plot parametri
  kFmiSpMatrixDataDraw =
      9998  // isolineview-luokalla piirrett‰v‰n matriisi-dataan liittyv‰ piirto-parametri
};

}  // end of namespace NFmiInfoData

#endif  // NFMIINFODATA_H
