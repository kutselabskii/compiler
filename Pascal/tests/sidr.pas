program project1;

var

x, y, z, max, min, maxnum, minnum,mid: byte;
f: text;


begin
assign(f,'input.txt');
reset(f);
read(f, x, y, z);
close(f);
assign(f, 'output.txt');
rewrite(f);

if (x=y) and (x=z) and (z=y) then
begin
writeln(f, 0);
close(f);
end

else
if (x+y+z) mod 3 <> 0 then
begin
writeln(f,-1);
close(f);
end
else
if (x>y) and (x>z) then
begin
max := x;
maxnum := 1;
if (z<y) then
begin
min := z;
minnum := 3;
mid :=y;
end
else
begin
min := y;
minnum:= 2;
mid := z;
end;

end;


if (y>x) and (y>z) then
begin
max:=y;
maxnum := 2;
if (x>z) then
begin
min := z;
minnum :=3;
mid := x;
end
else
begin
min := x;
minnum := 1;
mid := z;
end;
end;


if (z>x) and (z>y) then
begin
max := z;
maxnum := 3;
if (x>y) then
begin
min:=y;
minnum := 2;
mid := x;
end
else
begin
min := x;
minnum := 1;
min := y;
end;
end;


writeln(f,' ', maxnum,' ', minnum,' ', (max-mid));
close(f);




end.