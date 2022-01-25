[English](https://github.com/ic005k/QtOpenCoreConfig/blob/master/READMe.md) | [简体中文](https://github.com/ic005k/QtOpenCoreConfig/blob/master/READMe-cn.md) | [Italiano](https://github.com/ic005k/QtOpenCoreConfig/blob/master/READMe-it.md)
## Strumenti ausiliari OC: strumenti ausiliari OpenCore multipiattaforma open source


| [Release][release-link]|[Download][download-link]|[Issues][issues-link]|
|-----------------|-----------------|-----------------|
|[![release-badge](https://img.shields.io/github/release/ic005k/QtOpenCoreConfig.svg?style=flat-square "Release status")](https://github.com/ic005k/QtOpenCoreConfig/releases "Release status") | [![download-badge](https://img.shields.io/github/downloads/ic005k/QtOpenCoreConfig/total.svg?style=flat-square "Download status")](https://github.com/ic005k/QtOpenCoreConfig/releases/latest "Download status")|[![issues-badge](https://img.shields.io/badge/github-issues-red.svg?maxAge=60 "Issues")](https://github.com/ic005k/QtOpenCoreConfig/issues "Issues")|

[![Windows](https://github.com/ic005k/QtOpenCoreConfig/actions/workflows/windows.yml/badge.svg)](https://github.com/ic005k/QtOpenCoreConfig/actions/workflows/windows.yml)      [![MacOS](https://github.com/ic005k/QtOpenCoreConfig/actions/workflows/macos.yml/badge.svg)](https://github.com/ic005k/QtOpenCoreConfig/actions/workflows/macos.yml)       [![MacOS Classical](https://github.com/ic005k/QtOpenCoreConfig/actions/workflows/macos1012.yml/badge.svg)](https://github.com/ic005k/QtOpenCoreConfig/actions/workflows/macos1012.yml)  [![Ubuntu](https://github.com/ic005k/QtOpenCoreConfig/actions/workflows/ubuntu.yml/badge.svg)](https://github.com/ic005k/QtOpenCoreConfig/actions/workflows/ubuntu.yml)    

[download-link]: https://github.com/ic005k/QtOpenCoreConfig/releases/latest "Download status"
[download-badge]: https://img.shields.io/github/downloads/ic005k/QtOpenCoreConfig/total.svg?style=flat-square "Download status"

[release-link]: https://github.com/ic005k/QtOpenCoreConfig/releases "Release status"
[release-badge]: https://img.shields.io/github/release/ic005k/QtOpenCoreConfig.svg?style=flat-square "Release status"

[issues-link]: https://github.com/ic005k/QtOpenCoreConfig/issues "Issues"
[issues-badge]: https://img.shields.io/badge/github-issues-red.svg?maxAge=60 "Issues"

[discourse-link]: https://www.insanelymac.com/forum/topic/344752-open-source-cross-platform-opencore-auxiliary-tools/
[discourse-link]: https://www.macos86.it/topic/5316-ocat-discussione-e-approfondimenti/



OC Auxiliary Tool (OCAT o ocat in breve) e' un'utilita' molto semplice per OC, ricca di funzionalita' e facile da usare. OCAT mantiene la sua natura strutturale di essere completamente trasparente a livello utente, con un design completamente aperto, e' ancora in evoluzione e accoglie qualsiasi domanda o suggerimento.

* [Inglese: Aggiornare OpenCore e i Kexts con OCAT (da @5T33Z0)](https://github.com/5T33Z0/OC-Little-Translated/blob/main/D_Updating_OpenCore/README.md)

* [Italiano: Aggiornare OpenCore e i Kexts con OCAT (da @antuneddu)](https://www.macos86.it/topic/5238-aggiornare%C2%A0-opencore-e-kexts-con-ocat/)

* I contenuti nei Preset per i Quirks sono forniti e mantenuti by @5T33Z0.  [Dettagli](https://github.com/5T33Z0/OC-Little-Translated/tree/main/F_Desktop_EFIs/preset)

* Il pacchetto plugin Intel BaseConfigs e' fornito e mantenuto da @5T33Z0. [Dettagli](https://github.com/5T33Z0/OC-Little-Translated/tree/main/F_Desktop_EFIs)

* I contenuti dei Preset, quirk, BaseConfigs, AMD kernel patch preset per tutte le CPU AMD e la traduzione in Italiano di questo README sono forniti e mantenuti da [@fabiosun (macOS86.it)](https://github.com/macos86/QtOpenCoreConfig).

---

### Ringraziamenti speciali
* [@vit9696](https://github.com/vit9696) Suggerimenti per la funzionalità' di OCAT.
* [@5T33Z0](https://github.com/5T33Z0) I pacchetti di plug-in OCAT sono forniti e mantenuti.
* [@LucasMucGH](https://github.com/LucasMucGH) Suggerimenti per la progettazione dell'interfaccia utente nativa basati sulla versione per Mac.
* [@fabiosun (macOS86.it)](https://github.com/macos86/QtOpenCoreConfig) per i contenuti relativi a CPU AMD.

---

### Come configurare una nuova EFI?
* Il processo e' molto semplice, clicca sulla barra menu "Database", doppio click sul modello che corrisponde al tuo nella lista del database (grazie a @5T33Z0 per fornire e mantenere questi file), una cartella EFI sara' automaticamente creata sul desktop ed il processo e' cosi' completato.

### Come aggiornare automaticamente OC? (facile, sono necessari solo due passi)
* Trascina e rilascia il file di configurazione nell'interfaccia di OCAT e scegli "salva" dal menu.
* Successivamente seleziona "Upgrade OC Main Program" sulla toolbar, una interfaccia "Synchronize OC" comparirà, Premi il bottone in basso "Start Sync" per completare l'aggiornamento.

### Perche' l'icona di verifica di OC diventa rossa quando apro o salvo il file di configurazione?
* Perche' c'e' un problema con il file di configurazione, di solito diventa normale dopo il salvataggio del file di configurazione. Se viene mostrato ancora un problema dopo il salvataggio del file, e' molto semplice il processo per risolvere il problema , seguendo i messaggi di verifica che compaiono.

### Quali sono le operazioni convenienti da fare in OCAT?
* Tutti I files che necessitano di essere aperti possono essere aperti trascinandoli e rilasciandoli nella corrispondente interfaccia (supportata la selezione singola e multipla).
* La ricerca nella barra degli strumenti (si consiglia vivamente di utilizzare il portale di ricerca quando si modificano i file di configurazione invece di fare clic sulle schede una per una, supporta il matching fuzzy ed e' molto potente).
* Premi sinistro e trascina il mouse all'interno della tabella dati per effettuare selezioni multiple.
* Apri due o piu' profili per confrontare, copiare o incollare righe tra tabelle di dati (supporta la selezione singola o multipla).
* ...

### Come scarica OCAT OpenCore?
* OCAT correntemente prende OpenCore da OC, da questo indirizzo: https://github.com/acidanthera/OpenCorePkg/actions che e' "OpenCore Factory" nel menu "Help" di OCAT. Oltre a questo, OCAT non prende nessun files per OpenCore da altre fonti.
* Nota bene: tutti i materiali raw usati in OCAT provengono da fonti ufficiali di OpenCore, OCAT non prende contenuti da terze parti, ne ora, ne mai!.

### Perche' OCAT non usa un approccio online per ottenere automaticamente OC?
* Perche' OC e' rilasciato in versione stabile una volta al mese, un update di OC e' accompagnato inevitabilmente da un update di OCAT, non e' necessario in quanto c'e' tutto il tempo per aggiornare senza usare il metodo fetch.

---

### Grazie per il supporto ai seguenti software open source!

[OpenCore](https://github.com/acidanthera/OpenCorePkg)&nbsp; &nbsp; &nbsp; &nbsp;
[qtplist](https://github.com/reillywatson/qtplist)&nbsp; &nbsp; &nbsp; &nbsp;
[FindESP](https://github.com/bluer007/FindESP)&nbsp; &nbsp; &nbsp; &nbsp;
[winfile](https://github.com/microsoft/winfile)&nbsp; &nbsp; &nbsp; &nbsp;
[PlistCpp](https://github.com/animetrics/PlistCpp)&nbsp; &nbsp; &nbsp; &nbsp;
[pugixml](https://github.com/zeux/pugixml)&nbsp;&nbsp; &nbsp; &nbsp;
[aria2](https://github.com/aria2/aria2)&nbsp; &nbsp; &nbsp;&nbsp;
[wget](http://wget.addictivecode.org/)&nbsp; &nbsp; &nbsp;&nbsp;

### E alla fine , un grande ringraziamento alle persone che hanno contribuito, anche se non menzionate qui, per il vostro supporto e partecipazione!

---

### Nota: Questo progetto e' iniziato ad Agosto 2020
API: https://api.github.com/repos/ic005k/QtOpenCoreConfig/releases/latest
