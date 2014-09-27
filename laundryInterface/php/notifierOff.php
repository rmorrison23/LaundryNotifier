<?php

	$fp = fopen('../data/xivelyFeed.json', 'w');
    fwrite($fp, "{\"version\":\"1.0.0\",\"datastreams\":[{\"id\":\"Notifier\",\"current_value\":\"0\"}]}");
    fclose($fp);
	
	$output = shell_exec("./scripts/sendXivelyFeed.sh");
    //echo "<pre>$output</pre>";

	$fp = fopen('../data/notifierStatus.txt', 'w');
    fwrite($fp, "*Notifier Off\n");
    fclose($fp);

?>