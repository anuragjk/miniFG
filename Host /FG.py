import serial

ser = serial.Serial( raw_input('Enter port name: '), input('Enter baudrate: ') )
while(True):
	str = ''
	str = str+'%s'%(['SIN','SQR','TRI'][ input('Type of wave\n------------\n\n1.SIN\n2.SQR\n3.TRI\n:') - 1 ])
	str = str+'*'+'%d'%(input('Enter Amplitude (V): '))
	str = str+'*'+'%d*'%(input('Enter Frequency (Hz): '))
	ser.write(str)
	raw_input('Enter any key to stop')
	str = 'STOP*'
	ser.write(str)

