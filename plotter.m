clear all;
close all;

i = 1;

memsize = log10([100, 500, 1000, 5000, 10000, 50000, 100000, 500000, 1000000, 5000000, 5400000]); %/ 1000;  
memsizePy = log10([100, 500, 1000, 5000, 10000, 50000, 100000, 500000, 1000000]);% / 1000;

d2 = 1:6;
d3 = 7:12;
d4 = 13:18;

d = 1:11;

csharp_mono = dlmread('results-csharp-mono.txt', ';');
csharp_net = dlmread('results-csharp-net.txt', ';');
%java = dlmread('results-java.txt', ';');
clang = dlmread('results-clang.txt', ';');
%python = dlmread('results-python.txt', ';');
%orig = dlmread('results-orig.txt', ';');

figure('name', 'Ovisnost potrosnje memorije o ulaznom nizu');
hold on;
grid on;
title('Ovisnost potrosnje memorije o ulaznom nizu');
plot(memsize, csharp_mono(d, 4));
plot(memsize, csharp_net(d, 4), 'r');
%plot(memsize, java(d, 4), 'r');
plot(memsize, clang(d, 4), 'g');
%plot(memsizePy, python(1:9, 4),'y');
%plot(memsize, orig(1:11, 4), 'black');
legend('csharp-mono', 'csharp-net', 'clang');
ylabel('kB');
xlabel('log10(ulazni niz) (B)');

figure('name', 'Ovisnost vremena izvodjenja o ulaznom nizu');
title('Ovisnost vremena izvodjenja o ulaznom nizu');
hold on;
grid on;
plot(memsize, csharp_mono(d, 1));
plot(memsize, csharp_net(d, 1), 'r');
%plot(memsize, java(d, 1), 'r');
plot(memsize, clang(d, 1), 'g');
%plot(memsizePy, python(1:9, 1),'y');
%plot(memsize, orig(1:11, 1), 'black');
legend('csharp-mono', 'csharp-net', 'clang');
xlabel('log10(ulazni niz) (B)');
ylabel('sec');