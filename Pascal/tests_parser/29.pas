program abc;

const b = 1;
      q = 30;
      bbb = -100;

type mytype = integer;

var a, b, c: integer;
    d, c, qwerty: real;
    mas: array [1..10] of array [1..10] of integer;
    rec: record
    	a: integer;
    	b: real;
    	c: record
		a: array [1..100, 1..10] of record
			qwerty: real;
			iii: array[0..100] of integer;
			end;
   	end;
   end;

begin
a := 5 + 18 + 30 - 144;
b := a * 0 + (333 - 120) / 12;
qwerty := a div b;

begin
repeat
	a := a + b;
	b := b - a;
until (b < a);
end;

if (a + b = b - a) then
	begin
		a := a + b;
	end;

if (a) then 
	a := a + b
else
	b := b + a;

qwerty := d / c;
end.