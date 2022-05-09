[English](https://github.com/ic005k/QtOpenCoreConfig/blob/master/READMe.md) | [简体中文](https://github.com/ic005k/QtOpenCoreConfig/blob/master/READMe-cn.md) | [Italiano](https://github.com/ic005k/QtOpenCoreConfig/blob/master/READMe-it.md)

# Strumenti ausiliari per OpenCore (OCAT)

![GUI](https://user-images.githubusercontent.com/76865553/165901706-abbccb4a-89bc-4b03-b6e5-f52dcb10b53c.png)

## About
OpenCore Auxiliary Tools è un configuratore basato su GUI per la modifica del file `config.plist` per il bootmanager di acidanthera.

A differenza di altre applicazioni, OCAT non rovina la configurazione se gli sviluppatori di OpenCore aggiungono nuove funzionalità/chiavi alla struttura del file di configurazione. Invece, adatta e integra automaticamente nell'interfaccia le nuove funzionalità.

## Caratteristiche

Oltre ad essere un configuratore di plist può fare molto di più:

* Monta la partizione EFI e apri automaticamente config.plis
* Aggiorna e migra le configurazioni alle specifiche più recenti semplicemente facendo clic sul pulsante "Salva". Non è più necessario copiare le chiavi e convalidare manualmente la configurazione (enorme risparmio di tempo).
* Esegue automaticamente la convalida della configurazione e indica possibili conflitti/problemi di configurazione.
* Funzione di sincronizzazione: verifica e applica gli aggiornamenti per OpenCore, Resources e Kext con pochi clic
* Scegli tra la versione ufficiale/dev e le build di debug di OpenCore
* Menu a discesa con quirk suggeriti per CPU Intel e AMD
* Menu preimpostati per ACPI, kernel e altri elenchi.
* Elenco modificabile con URL delle repo dei Kext
* Convertitore HEX ASCII integrato
* Database con configurazioni di base complete per CPU Intel e AMD basate sulla Guida all'installazione di OpenCore di Dortania
* Possibilità di generare cartelle EFI da una configurazione con un solo clic
* Possibilità di creare backup di cartelle EFI

## Guide

* [Aggiornare OpenCore e kext con OCAT (5T33Z0)](https://github.com/5T33Z0/OC-Little-Translated/blob/main/D_Updating_OpenCore/README.md)
* Il contenuto del plug-in preselezionato per Intel CPU Quirks è fornito e gestito da 5T33Z0. [Dettagli](https://github.com/5T33Z0/OC-Little-Translated/tree/main/F_Desktop_EFIs/preset)
* Pacchetto plug-in Intel CPU BaseConfigs fornito e gestito da 5T33Z0. [Dettagli](https://github.com/5T33Z0/OC-Little-Translated/tree/main/F_Desktop_EFIs)
* Pacchetto plug-in AMD CPU BaseConfigs, preselezionato per AMD CPU Quirks, pacchetto di patch del kernel AMD, documentazione descrittiva in italiano fornita e mantenuta da [fabiosun](https://github.com/fabiosun).
* [Guida per gli strumenti ausiliari di OpenCore (chriswayg)](https://chriswayg.gitbook.io/opencore-visual-beginners-guide/oc_auxiliary_tools)


## Credits
* [vit9696](https://github.com/vit9696) Suggerimenti di funzionalità OCAT ecc.
* [5T33Z0](https://github.com/5T33Z0) Pacchetto plug-in CPU Intel per OCAT, suggerimenti per l'interazione e la funzionalità dell'utente, ecc.
* [LucasMucGH](https://github.com/LucasMucGH) Suggerimenti per la progettazione dell'interfaccia utente nativa per versioni basate su Mac, ecc.
* [fabiosun](https://github.com/fabiosun) Contenuti relativi ad AMD.
* [chriswayg](https://github.com/chriswayg) Guida per l'utente degli strumenti ausiliari OpenCore.
* APP Icon Design: Mirone (Brasile).
* [OpenCore](https://github.com/acidanthera/OpenCorePkg)&nbsp; &nbsp; &nbsp; &nbsp;
[qtplist](https://github.com/reillywatson/qtplist)&nbsp; &nbsp; &nbsp; &nbsp;
[FindESP](https://github.com/bluer007/FindESP)&nbsp; &nbsp; &nbsp; &nbsp;
[winfile](https://github.com/microsoft/winfile)&nbsp; &nbsp; &nbsp; &nbsp;
[PlistCpp](https://github.com/animetrics/PlistCpp)&nbsp; &nbsp; &nbsp; &nbsp;
[pugixml](https://github.com/zeux/pugixml)&nbsp;&nbsp; &nbsp; &nbsp;
[aria2](https://github.com/aria2/aria2)&nbsp; &nbsp; &nbsp;&nbsp;
[wget](http://wget.addictivecode.org/)&nbsp; &nbsp; &nbsp;&nbsp;
[DirectionalToolTip](https://github.com/scondratev/DirectionalToolTip)&nbsp; &nbsp; &nbsp;&nbsp;
[dortania build-repo](https://github.com/dortania/build-repo)&nbsp; &nbsp; &nbsp;&nbsp;
[HackinPlugins](https://github.com/bugprogrammer/HackinPlugins)&nbsp; &nbsp; &nbsp;&nbsp;

---

API: https://api.github.com/repos/ic005k/OCAuxiliaryTools/releases/latest
