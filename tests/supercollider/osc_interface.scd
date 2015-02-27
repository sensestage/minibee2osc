// "pydongui.py".runInTerminal;
// check SC's osc port (use as argument for minihiveosc
NetAddr.langPort;

OSCFunc.trace();

OSCFunc.trace( false );



OSCdef( \minibeedata, { |data,time,src|  data.postln; }, '/minibee/data' );
OSCdef( \minibeeinfo, { |data,time,src| [data,time,src].postln; }, '/minibee/info' );
OSCdef( \minibeestatus, { |data,time,src| [data,time,src].postln; }, '/minibee/status' );

OSCdef( \minibeedata ).free;


n = NetAddr.new( "127.0.0.1" , 57600 );

(
// set output to the minibee:
Tdef( \sendoutput, {
	loop{
		"sending message".postln;
		n.sendMsg( "/minibee/output", 1, 255 ); // arguments: minibee ID, value of the led output
		0.05.wait;
	}
});
)

Tdef( \sendoutput ).play;
Tdef( \sendoutput ).stop;

n.sendMsg( "/minibee/reset", 1 ); // not working yet
n.sendMsg( "/minibee/announce", 1 );
n.sendMsg( "/minibee/saveid", 1 );

n.sendMsg( "/minibee/run", 1, 1 );
n.sendMsg( "/minibee/run", 1, 0 );

n.sendMsg( "/minibee/loopback", 1, 1 );
n.sendMsg( "/minibee/loopback", 1, 0 );

n.sendMsg( "/minibee/custom", 1, 255 );
n.sendMsg( "/minibee/custom", 1, 255, 255 );


// number of values you send needs to match the number of outputs you defined:
// the order of the values:
// first PWM / AnalogOut pins (D9)
// first DigitalOut pins (D3)
