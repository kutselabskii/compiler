program abc;
uses crt;
var a, b: array[1..100] of integer;
begin
a[0] := 1;
b[1] := 117;
writeln (a[0] + b[1]);
readkey;
end.