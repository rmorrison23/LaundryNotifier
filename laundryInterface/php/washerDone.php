<?php

	$fp = fopen('../data/xivelyFeed.json', 'w');
    fwrite($fp, "{\"version\":\"1.0.0\",\"datastreams\":[{\"id\":\"Washer\",\"current_value\":\"0\"}]}");
    fclose($fp);

	$fp = fopen('../data/machineStatus.txt', 'w');
	fwrite($fp, "*Washer Off\n");
	fclose($fp);
		
	$output = shell_exec("./scripts/sendXivelyFeed.sh");
    //echo "<pre>$output</pre>";

	$output = shell_exec("./scripts/textWasherDone.sh");

?>