<?php

	$fp = fopen('../data/xivelyFeed.json', 'w');
    fwrite($fp, "{\"version\":\"1.0.0\",\"datastreams\":[{\"id\":\"Washer\",\"current_value\":\"0\"}]}");
    fclose($fp);
	
	$output = shell_exec("./scripts/sendXivelyFeed.sh");
    //echo "<pre>$output</pre>";

    $fp = fopen('../data/xivelyFeed.json', 'w');
    fwrite($fp, "{\"version\":\"1.0.0\",\"datastreams\":[{\"id\":\"Dryer\",\"current_value\":\"0\"}]}");
    fclose($fp);
	
	$output = shell_exec("./scripts/sendXivelyFeed.sh");
    //echo "<pre>$output</pre>";

    $fp = fopen('../data/xivelyFeed.json', 'w');
    fwrite($fp, "{\"version\":\"1.0.0\",\"datastreams\":[{\"id\":\"Both\",\"current_value\":\"0\"}]}");
    fclose($fp);
	
	$output = shell_exec("./scripts/sendXivelyFeed.sh");
    //echo "<pre>$output</pre>";
?>