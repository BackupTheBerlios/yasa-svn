// STG in JSON Format
var stg =
[
{ 
    // index:0,
	duration:3,
	pred:[]
},
{
	// index:1,
	duration:5,
	pred:[{index:0,time:2}]
},
{
	// index:2,
	duration:2,
	pred:[{index:0,time:1}]
},
{
	// index:3,
	duration:2,
	pred:[{index:0,time:3}]
},
{
	// index:4,
	duration:4,
	pred:[{index:0,time:4}]
},
{
	// index:5,
	duration:1,
	pred:[ {index:1,time:2},
		   {index:2,time:1},
		   {index:3,time:1},
		   {index:4,time:2}
		 ]
}
];

// Finde loose Enden, d.h. Knoten ohne (nicht bereits entnommene) 
// Nachfolger
function find_end_nodes(stg,scheduled)
{
	var i,j;	
	var task;
	
	// Nehme vorerst an, jeder Knoten ist ein Ende  
	var end_node = [];
	for (i=0; i<stg.length; i++) end_node[i] = true;	
	
	// Für jeden Task
	for (i=0;i<stg.length; i++)
	{
		// ... ausser denen, die bereits verplant sind ...
		if (scheduled[i]) continue;
		
		// ... durchlaufe alle Vorgänger
		var preds = stg[i].pred;
		for (j=0; j<preds.length; j++)
		{
			// und markiere als 'kein Ende'
			var index = preds[j].index;						
			end_node[index] = false;
		}
	}
		
	var loose_ends = [];	
	for (i=0; i<end_node.length; i++)
	{
		if ((end_node[i])&&(!scheduled[i])) loose_ends.push(i);
	}
	
	return loose_ends;
}

// Transformiere Lösung ('indirect representation')
// in Schedule
function transform(x,stg,num_procs)
{	
	var i,j;
	
	// Initialisiere Datenstruktur für Schedule
	var proc_tasks = [];
	// Ein Array für jeden Proc
	for (i=0; i<num_procs; i++) proc_tasks[i] = [];	
	
	// Verplante Tasks (=entnommene Knoten)
	var scheduled = [];
	for (i=0; i<stg.length; i++) scheduled[i] = false;
	
	// Ordne alle Tasks in Schedule ein
	for (i=0; i<stg.length; i++) 
	{
		var end_nodes = find_end_nodes(stg, scheduled);

		// Wähle den Task mit der höchsten Ordnung aus.
		var index = end_nodes[0];
		var max_order = x[index].order;
		
		for (j=1; j<end_nodes.length; j++)
		{			
			var n = end_nodes[j];
			if (x[n].order>max_order) 
			{
				max_order = x[n].order;
				index = n;
			}
		}
		
		// Füge in Schedule ein
		var proc = x[index].proc;
		proc_tasks[proc].unshift(index); // Setze an den Anfang 
		
		// Markiere als verplant
		scheduled[index] = true;
	}
	
	return proc_tasks;
}


// --------------------------------------------------


// Speichere Kommunikationszeiten in Matrix
function com_matrix(stg)
{
	var com_time = [];
	var len = stg.length;
	
	var i,j;
	for (i=0; i<len; i++)
	{
		com_time[i] = [];
		for (j=0; j<len; j++) com_time[i][j] = 0;
		
		var task = stg[i];
		for (j=0; j<task.pred.length; j++)
		{
			var pred = task.pred[j];
			com_time[pred.index][i] = pred.time;
		}
	}
	
	return com_time;
}

function pos_of_task(schedule)
{
	var pos = [];
	for (var p=0;p<schedule.length;p++)
	{
		for (var t=0;t<schedule[p].length;t++)
		{
			var index = schedule[p][t];
			pos[index] = {proc:p,order:t};
		}
	}
	
	return pos;
}

function schedule_times(stg,schedule)
{
	var com_time = com_matrix(stg);
	var plan = pos_of_task(schedule);
		
	// Endzeit
	var end = [];
	for (var i=0; i<stg.length; i++) end[i] = -1;
	
	function calc_end(index)
	{
		var start = 0;
		
		var pred = stg[i].pred;
		var pos = plan[index];
		var proc = pos.proc;
		
		// nicht der erste Task auf dem Proc.
		// -> Vorgänger berücksichtigen
		if (pos.order>0)
		{
			// Vorgänger auf Proc.
			var prev = schedule[pos.proc][pos.order-1];
			if (end[prev]==-1) calc_end(prev);
			
			start = end[prev];
		}
		
		// STG Vorgänger
		for (var i=0; i<pred.length; i++)
		{
			var index_p = pred[i].index;
			if (end[index_p]==-1) calc_end(index_p);			
			
			var time = end[index_p];
			
			// Anderer Proc -> Kommunikationszeit dazu
			if (plan[index_p].proc!=proc)
			{							
				time += com_time[index_p][index];			
			}
			
			if (time>start) start = time;
		}
		
		end[index] = start + stg[index].duration;
	}
	
	for (var i=0; i<stg.length; i++)
	{
		if (end[i]>-1) continue; // bereits bekannt
		calc_end(i);
	}
	
	return end;
}


// Eine beliebige Lösung (indirect representation)
var proc_count = 2;
var x = [{proc:0,order:1},
		 {proc:1,order:4},
		 {proc:1,order:3},
		 {proc:0,order:2},
		 {proc:0,order:5},
		 {proc:1,order:0}
		 ];

// Schedule (direct representation)
var schedule = transform(x,stg,proc_count);

for (var i=0; i<schedule.length; i++)
{
	trace("proc #"+i+": "+schedule[i]);
}

trace(schedule_times(stg,schedule));

