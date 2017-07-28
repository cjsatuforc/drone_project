figure(1)
subplot(311)
plot(t,x_d,'r')
hold on
grid on
ylabel('X[m]')
plot(t,x,'b')
subplot(312)
plot(t,y_d,'r')
hold on
grid on
ylabel('Y[m]')
plot(t,y,'b')
subplot(313)
plot(t,z_d,'r')
hold on
grid on
ylabel('Z[m]')
xlabel('Time[s]')
plot(t,z,'b')

figure(2)
plot3(x_d,y_d,z_d,'r')
hold on
grid on
plot3(x,y,z,'b')
xlabel('X[m]')
ylabel('Y[m]')
zlabel('Z[m]')