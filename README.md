# json-beta
error handler per frt configurabile in json

la gestione degli errori è implementata attraverso classi ```Responder_base```, utilizzabile come modulo all'interno di altre classi responder, sono qui implementate le classi
 - ```Responder_base``` come kernel degli altri responder
 - ```Traj_responder``` per gli errori di trajectory
 - e basta

per i concetti generali nel prgetto vedere [concetti](#concetti)
 
per l'integrazione dell'error handler con altri nodi ros vedere [integrazione](#integrazione)

per la scrittura e modifica di file json di configurazoine vedere [configurazione](#configurazione)

per l'utilizzo delle classi presenti nel nodo responder vedere [metodi](#metodi)

## Concetti

la gestione di errori avviene tramite callback,

a ogni errore che può arrivare al nodo handler sarà quindi associata una funzione di callback,

per indicare questa nel file json, a ogni funzione sarà associata una stringa di id che verrà usata per rappresentarla in json

## Utilizzo

per funzionare, l'error handler dovrà iscriversi ai topic di errore degli altri nodi presenti, questi vanno indicati nel file ```launch/coso.launch``` presente all'interno del package ```responder```

effettuata l'iscrizione al topic, in caso di errore basterà inviare su questo un messaggio di tipo ```std_msgs::String``` contente l'id dell'errore

## Configurazione

la path ai file json contenti la specifica da seguire è data nel file ```coso.launch``` presente nel package ```responder```
la struttura del file json da seguire è la seguente

```json
{
	"errors" : [
		
		{
			"id" : "id dell'errore"
			"fun id" : "id della funzione che si vuole chiamare"
			"desc" : "(opzionale) descrizione dell'errore"
		},
		{
			"id" : "id di un'altro errore"
			"fun id" : "id della funzione che si vuole chiamare"
			"desc" : "(opzionale) descrizione dell'errore"
		},
		...
	]
}
```
sono al momento presenti file di esempio nella cartella ```src/responder/error-files/```

## Interfaccia

i responder non base presentano i seguenti metodi
 - ```<costruttore>(std::string file_path)``` crea il responder e associa gli errori ai callback come specificato nel file json trovato alla path data
 - ```respond_to(std::string err_id)``` esegue la funzoine di callback che è stato precedentemente associata all'errore identificato con id ```err_id```
 - ```follow_json(json j)``` associa gli id di funzione ed errore come indicati nel membro "errors" del json passato (prevede la struttura citata in [configurazione](#configurazione)
 - ```bind(std::string err_id, std::string fun_id)``` associa l'errore con id ```err_id``` con la funzione con id ```fun_id```, usato da ```follow_json```, che sarebbe da preferire.
