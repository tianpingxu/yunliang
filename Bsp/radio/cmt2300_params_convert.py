# coding=utf-8
import sys,os
import codecs
import re
		
def cmt2300_import_hex(strFile):
	input = codecs.open(strFile, 'r', 'utf-8')
	arr8 = [0] * 0x60
	count = 0
	
	for line in input:
		line = line.replace('\r', '').replace('\n', '').replace('\t', '').strip()
		if(line=="") or (line[0]==';') or (line[0]=='[') or (line.find('Addr')>=0):
			continue
		
		arr = re.findall('0[xX][0-9A-Fa-f]+', line)
		
		if len(arr) >= 2:
			addr = int(arr[0], 16)
			arr8[addr] = int(arr[1], 16)
			
	input.close()
	
	return arr8
	
def cmt2300_convert_hex(strSrcFile, strDstFile, strSubfix):
	output = codecs.open(strDstFile, 'w', 'utf-8')
	arr8 = cmt2300_import_hex(strSrcFile)
	
	output.write('#ifndef __CMT2300_PARAMS' +strSubfix.upper()+ '_H\r\n')
	output.write('#define __CMT2300_PARAMS' +strSubfix.upper()+ '_H\r\n')
	output.write('\r\n')
	
	output.write('/* [CMT Bank] */\r\n')
	output.write('const u8 g_cmt2300CmtBank' +strSubfix+ '[CMT2300_CMT_BANK_SIZE] = {\r\n')
	for i in range(0x00, 0x0C):
		str = "    0x%02X," %(arr8[i])
		output.write(str +'\r\n')
	output.write('};\r\n\r\n')
	
	output.write('/* [System Bank] */\r\n')
	output.write('const u8 g_cmt2300SystemBank' +strSubfix+ '[CMT2300_SYSTEM_BANK_SIZE] = {\r\n')
	for i in range(0x0C, 0x18):
		str = "    0x%02X," %(arr8[i])
		output.write(str +'\r\n')
	output.write('};\r\n\r\n')
	
	output.write('/* [Frequency Bank] */\r\n')
	output.write('const u8 g_cmt2300FrequencyBank' +strSubfix+ '[CMT2300_FREQUENCY_BANK_SIZE] = {\r\n')
	for i in range(0x18, 0x20):
		str = "    0x%02X," %(arr8[i])
		output.write(str +'\r\n')
	output.write('};\r\n\r\n')
	
	output.write('/* [Data Rate Bank] */\r\n')
	output.write('const u8 g_cmt2300DataRateBank' +strSubfix+ '[CMT2300_DATA_RATE_BANK_SIZE] = {\r\n')
	for i in range(0x20, 0x38):
		str = "    0x%02X," %(arr8[i])
		output.write(str +'\r\n')
	output.write('};\r\n\r\n')
	
	output.write('/* [Baseband Bank] */\r\n')
	output.write('const u8 g_cmt2300BasebandBank' +strSubfix+ '[CMT2300_BASEBAND_BANK_SIZE] = {\r\n')
	for i in range(0x38, 0x55):
		str = "    0x%02X," %(arr8[i])
		output.write(str +'\r\n')
	output.write('};\r\n\r\n')
	
	output.write('/* [Tx Bank] */\r\n')
	output.write('const u8 g_cmt2300TxBank' +strSubfix+ '[CMT2300_TX_BANK_SIZE] = {\r\n')
	for i in range(0x55, 0x60):
		str = "    0x%02X," %(arr8[i])
		output.write(str +'\r\n')
	output.write('};\r\n\r\n')
	
	output.write('#endif\r\n')
	
	output.close()
	
cmt2300_convert_hex('cmt2300_params.exp', 'cmt2300_params.h', '')