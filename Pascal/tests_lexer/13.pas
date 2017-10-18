program project1;

var

x, y, z, max, min, maxnum, minnum,mid: byte;
f: text;


begin
assign(f,'input.txt');
reset(f);#
read($f, x, y, z);
close(f);
assign(f, 'output.txt');
rewrite(f);

if (x=y) and (x=z) and (z=y) then
begin
writeln(f, 0..1);
close(f..f);
end
end.