function machineSelect(clicked_id){

	if(clicked_id == "1"){
		var updateVars = $.ajax({
			url: "php/updateMachine.php",
			type: "POST",
			data:{washer: "ON", dryer: "OFF", both: "NO"},
			success: function(data){
			//console.log(data);
			alert("Washer is selected! \nPress run to initiate notifier.");
		}

		});

		var request = $.ajax({
        	url: "php/updateMachine.php",
        	type: "GET",
        	dataType: "html"
    	});
	}

	if(clicked_id == "2"){
		var updateVars = $.ajax({
			url: "php/updateMachine.php",
			type: "POST",
			data:{washer: "OFF", dryer: "ON", both: "NO"},
			success: function(data){
			//console.log(data);
			alert("Dryer is selected! \nPress run to initiate notifier.");
		}

		});

		var request = $.ajax({
        	url: "php/updateMachine.php",
        	type: "GET",
        	dataType: "html"
    	});
	}

	if(clicked_id == "3"){
		var updateVars = $.ajax({
			url: "php/updateMachine.php",
			type: "POST",
			data:{washer: "ON", dryer: "ON", both: "YES"},
			success: function(data){
			//console.log(data);
			alert("Both machines are selected! \nPress run to initiate notifier.");
		}

		});

		var request = $.ajax({
        	url: "php/updateMachine.php",
        	type: "GET",
        	dataType: "html"
    	});
	}	

}

function setNotifier() {

	var updateVars = $.ajax({
		url: "php/updateMachine.php",
		type: "POST",
		data:{notifier: "SET"},
		success: function(data){
			//console.log(data);
			alert("Notifier is set! \nYou'll be texted when the laundry is done.");
		}

	});

	var request = $.ajax({
        url: "php/updateMachine.php",
        type: "GET",
        dataType: "html"
    });

}