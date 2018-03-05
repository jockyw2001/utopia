import sys
import xlrd

###enter file name
#excel_filename_PWS_table=raw_input('Enter PWS excel fle:')
#if excel_filename_PWS_table == '' :
#    print "Input Error....\n"
#    sys.exit()

###Excel file handle
# book_pws = xlrd.open_workbook(excel_filename_PWS_table)
book_pws = xlrd.open_workbook("titania9_Power_Setting_04292010.xls")
#print "The number of sheets is ", book_pws.nsheets
#print "Sheet(s) name(s):", book_pws.sheet_names()

#get ver info sheet
sheet_change_history = book_pws.sheet_by_name("Change_History")
pws_table_version = sheet_change_history.col_values(0, 1, None)
pws_table_date = sheet_change_history.col_values(1, 1, None)
pws_table_version_length = len(pws_table_version)-1
#pws_table_ver = pws_table_version[pws_table_version_length] +'_'+ pws_table_date[pws_table_version_length]
pws_table_ver = str(pws_table_version[pws_table_version_length]) +'_'+ str(pws_table_date[pws_table_version_length])
print "Chage history info: "
print pws_table_ver

#get sheet info sheet
sheet_list_info = book_pws.sheet_by_name("Sheet_Info")

#get IP list
sheet_IP_list = sheet_list_info.col_values(0, 2, None)
sheet_IP_list_length = len(sheet_IP_list)-1
#remove empty string in sheet_IP_list
while sheet_IP_list_length >= 0:
    if sheet_IP_list[sheet_IP_list_length] == "":
        del sheet_IP_list[sheet_IP_list_length]
    sheet_IP_list_length = sheet_IP_list_length -1
sheet_IP_list_length = len(sheet_IP_list)-1
print "Sheet IP list info: "
print sheet_IP_list

#get Input source list
sheet_Input_list_useCase = sheet_list_info.col_values(1, 2, None)
sheet_Input_list_useCase_length = len(sheet_Input_list_useCase)-1
sheet_Input_list = sheet_list_info.col_values(2, 2, None)
sheet_Input_list_length = len(sheet_Input_list)-1
sheet_Input_list_ref = sheet_list_info.col_values(3, 2, None)
sheet_Input_list_ref_length = len(sheet_Input_list_ref)-1
#remove empty string in sheet_Input_list_useCase_length
while sheet_Input_list_useCase_length >= 0:
    if sheet_Input_list_useCase[sheet_Input_list_useCase_length] == "":
        del sheet_Input_list_useCase[sheet_Input_list_useCase_length]
    sheet_Input_list_useCase_length = sheet_Input_list_useCase_length -1
sheet_Input_list_useCase_length = len(sheet_Input_list_useCase)-1
#remove empty string in sheet_Input_list_length
while sheet_Input_list_length >= 0:
    if sheet_Input_list[sheet_Input_list_length] == "":
        del sheet_Input_list[sheet_Input_list_length]
    sheet_Input_list_length = sheet_Input_list_length -1
sheet_Input_list_length = len(sheet_Input_list)-1
#remove empty string in sheet_Input_list_ref_length
while sheet_Input_list_ref_length >= 0:
    if sheet_Input_list_ref[sheet_Input_list_ref_length] == "":
        del sheet_Input_list_ref[sheet_Input_list_ref_length]
    sheet_Input_list_ref_length = sheet_Input_list_ref_length -1
sheet_Input_list_ref_length = len(sheet_Input_list_ref)-1
#print "Sheet Input list info: "
#print sheet_Input_list
#print "Sheet input list length:"
#print sheet_Input_list_length
#print "sheet_Input_list_useCase ", sheet_Input_list_useCase
#set Input source list ref directory type
input_source_ref = {}
input_source_ref_offset = 0
while sheet_Input_list_ref_length >= 0:
    input_source_ref[sheet_Input_list_ref[sheet_Input_list_ref_length]] = input_source_ref_offset
    input_source_ref_offset = input_source_ref_offset + 1
    sheet_Input_list_ref_length = sheet_Input_list_ref_length - 1
sheet_Input_list_ref_length = len(sheet_Input_list_ref)-1
#print "Sheet Input list info ref:", sheet_Input_list_ref
#print input_source_ref
#set Input source list directory type
input_source_list = {}
while sheet_Input_list_length >= 0:
    input_source_list[sheet_Input_list_useCase[sheet_Input_list_length]] = sheet_Input_list[sheet_Input_list_length]
    sheet_Input_list_length = sheet_Input_list_length - 1
sheet_Input_list_length = len(sheet_Input_list)-1
#print "input_source_list dir ", input_source_list
###text file handle
#open file (pws_setting_info_table.h)
pws_table_file = open('mhal_pws_setting_info_table.h','w')
pws_table_file.write('#ifndef _PWS_SETTING_INFO_TABLE_H_\n')
pws_table_file.write('#define _PWS_SETTING_INFO_TABLE_H_\n\n')
pws_table_file.write('#include \"MsTypes.h\"\n\n')

#write pws data structure
pws_table_file.write('typedef   struct   {\n')
pws_table_file.write('    MS_U32     u32RegAddr;\n')
pws_table_file.write('    MS_U8     u8RegMask;\n')
pws_table_file.write('    /*MS_U16     u16OffOnFlag;*/\n')
pws_table_file.write('    MS_BOOL     bInvert;\n')
pws_table_file.write('    const char regName[32];//*regName;\n')
pws_table_file.write('}PWS_TABLE_INFO;\n\n')

#write pws table info
pws_table_file.write('static const PWS_TABLE_INFO pws_setting_info_table[] =\n')
pws_table_file.write('{\n')
PWS_Items = 0 #total pws items
offOnBitmap_for_pws_table = [] #for pws table use include in api layer
regName_for_pws_table = [] #for pws table use include in api layer
offOnBitmap_regName_for_pws_table_index = 0

pws_init_table_IP_RegName = []  #for pws init table use that all OFF or ON
pws_init_table_IP_Address = []  #for pws init table use that all OFF or ON
pws_init_table_IP_Mask = []     #for pws init table use that all OFF or ON
pws_init_table_IP_Invert = []   #for pws init table use that all OFF or ON
pws_init_table_offOnBitmap = [] #for pws init table use that all OFF or ON
pws_init_regName_for_pws_table_index = 0

for x in sheet_IP_list:
    sheet_IP = book_pws.sheet_by_name(x)
    sheet_IP_Valid = sheet_IP.col_values(5, 2, None)
    sheet_IP_Invert = sheet_IP.col_values(4, 2, None)
    sheet_IP_RegName = sheet_IP.col_values(1, 2, None)
    sheet_IP_Address = sheet_IP.col_values(2, 2, None)
    sheet_IP_Mask = sheet_IP.col_values(3, 2, None)
    #for getting offOn flag, init value
    offOnBitmap = [] #OffOnFlag use
    offOnFlag_length = 0
    #offOnFlag_value = 0x0000
    sheet_IP_RegName_length = len(sheet_IP_RegName) -1 #zero-based

###to be finished
    #print "sheet_IP_list_length :", sheet_IP_list_length, "!!!"
    #print "sheet_Input_list_useCase_length :", sheet_Input_list_useCase_length
    ###sheet_IP_OffOnFlag_inputSource = sheet_IP.row_values(1, 6, (6+(sheet_IP_list_length+1))) #need modify
    sheet_IP_OffOnFlag_inputSource = sheet_IP.row_values(1, 6, (6+(sheet_Input_list_useCase_length+1))) #need modify
    #print "sheet_IP_OffOnFlag_inputSource ", sheet_IP_OffOnFlag_inputSource

    while offOnFlag_length <= sheet_IP_RegName_length :
        ###sheet_IP_OffOnFlag = sheet_IP.row_values((2+offOnFlag_length), 6, (6+(sheet_IP_list_length+1))) #need modify
        sheet_IP_OffOnFlag = sheet_IP.row_values((2+offOnFlag_length), 6, (6+(sheet_Input_list_useCase_length+1))) #need modify

        offOnBitmap_all_OFF_or_ON = [] #for calculate if all source is OFF or ON

        #print "sheet_IP_OffOnFlag ", sheet_IP_OffOnFlag, "IP list name", x
        #print "offOnFlag_length :",offOnFlag_length
        input_offOnFlag_length = 0
        #input_offOnFlag_shift_offset = sheet_Input_list_length
        #print "get input_source_ref dir value :", input_source_ref['CVBS']
###        if sheet_IP_Invert[offOnFlag_length] == "N" :
        offOnFlag_value = 0x0000    #off = 0x01, on = 0x00

###        else :
###            offOnFlag_value = 0xFFFF    #off = 0x00, on = 0x01

        #print "input_offOnFlag_length >>>", input_offOnFlag_length, "sheet_Input_list_length>>>", sheet_Input_list_length

        while input_offOnFlag_length <= sheet_Input_list_length:
            input_offOnFlag_shift_offset = input_source_ref[input_source_list[sheet_IP_OffOnFlag_inputSource[input_offOnFlag_length]]]
            #print "input_offOnFlag_shift_offset ", input_offOnFlag_shift_offset
            #print "input_source_ref ", input_source_ref
            #print "input_source_list ", input_source_list[sheet_IP_OffOnFlag_inputSource[input_offOnFlag_length]]
###            if sheet_IP_Invert[offOnFlag_length] == "N" : #off = 0x01, on = 0x00
###                #print "No invert"
            if sheet_IP_OffOnFlag[input_offOnFlag_length] == "ON" :#need modify
                offOnBitmap_all_OFF_or_ON.insert(input_offOnFlag_length, 0x00) ###ON
                #offOnFlag_value = offOnFlag_value & ~(0x01 << input_offOnFlag_length)
                offOnFlag_value = offOnFlag_value & ~(0x01 << input_offOnFlag_shift_offset)
            else : #need modify
                offOnBitmap_all_OFF_or_ON.insert(input_offOnFlag_length, 0x01) ###OFF
                #offOnFlag_value = offOnFlag_value | (0x01 << input_offOnFlag_length)
                offOnFlag_value = offOnFlag_value | (0x01 << input_offOnFlag_shift_offset)
###            else : #off = 0x00, on = 0x01
###                #print "Yes invert, "
###                if sheet_IP_OffOnFlag[input_offOnFlag_length] == "ON" :#need modify
###                    offOnBitmap_all_OFF_or_ON.insert(input_offOnFlag_length, 0x01) ###OFF
###                    #offOnFlag_value = offOnFlag_value | (0x01 << input_offOnFlag_length)
###                    offOnFlag_value = offOnFlag_value | (0x01 << input_offOnFlag_shift_offset)
###                 #print offOnFlag_value
###                else : #need modify
###                    offOnBitmap_all_OFF_or_ON.insert(input_offOnFlag_length, 0x00) ###OFF
###                    #offOnFlag_value = offOnFlag_value & ~(0x01 << input_offOnFlag_length)
###                    offOnFlag_value = offOnFlag_value & ~(0x01 << input_offOnFlag_shift_offset)
###                    #print offOnFlag_value
            #input_offOnFlag_shift_offset = input_offOnFlag_shift_offset - 1
            input_offOnFlag_length = input_offOnFlag_length + 1

        #print "sheet_IP_RegName>>>",sheet_IP_RegName[sheet_IP_RegName_length] ,", offOnBitmap_all_OFF_or_ON >>>>>>", offOnBitmap_all_OFF_or_ON
        offOnBitmap_all_OFF_or_ON_length = len(offOnBitmap_all_OFF_or_ON) - 1
        #print "offOnBitmap_all_OFF_or_ON_length >>>", offOnBitmap_all_OFF_or_ON_length
        offOnBitmap_all_OFF_or_ON_length_index = 0
        #print "offOnBitmap_all_OFF_or_ON_length_index>>", offOnBitmap_all_OFF_or_ON_length_index
        ###!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        #while offOnBitmap_all_OFF_or_ON_length_index <= offOnBitmap_all_OFF_or_ON_length:
        #    print "offOnBitmap_all_OFF_or_ON_length_index===>", offOnBitmap_all_OFF_or_ON_length_index, ", offOnBitmap_all_OFF_or_ON_length>>", offOnBitmap_all_OFF_or_ON_length
        #    print "offOnBitmap_all_OFF_or_ON[", offOnBitmap_all_OFF_or_ON_length_index ,"] >>> ", offOnBitmap_all_OFF_or_ON[offOnBitmap_all_OFF_or_ON_length_index]
        #    offOnBitmap_all_OFF_or_ON_length_index = offOnBitmap_all_OFF_or_ON_length_index + 1
        ###!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        #print "offOnBitmap_all_OFF_or_ON =====>>>>", offOnBitmap_all_OFF_or_ON
        offOnBitmap_all_OFF_or_ON_length_index = 0
        offOnBitmap_all_OFF_or_ON_length_index_next = 1
        isAll_Off_or_on = 1 #if all flag is off or on,
        while offOnBitmap_all_OFF_or_ON_length_index <= offOnBitmap_all_OFF_or_ON_length:
            if offOnBitmap_all_OFF_or_ON_length_index_next > offOnBitmap_all_OFF_or_ON_length:
                #print "break >>>"
                break
            if offOnBitmap_all_OFF_or_ON[offOnBitmap_all_OFF_or_ON_length_index] != offOnBitmap_all_OFF_or_ON[offOnBitmap_all_OFF_or_ON_length_index_next]:
                isAll_Off_or_on = 0
                #print "isAll_Off_or_on >>>", isAll_Off_or_on
            offOnBitmap_all_OFF_or_ON_length_index = offOnBitmap_all_OFF_or_ON_length_index + 1
            offOnBitmap_all_OFF_or_ON_length_index_next = offOnBitmap_all_OFF_or_ON_length_index_next + 1
#####
        if sheet_IP_Invert[offOnFlag_length] == "N" and isAll_Off_or_on==1: #no invert andd all off or on
            if offOnBitmap_all_OFF_or_ON[0] == 0: # means ON
                offOnFlag_value = 0x0000
            else:
                offOnFlag_value = 0xFFFF #means off
        if sheet_IP_Invert[offOnFlag_length] == "Y" and isAll_Off_or_on==1:
            if offOnBitmap_all_OFF_or_ON[0] == 1: # means ON
                offOnFlag_value = 0xFFFF
            else:
                offOnFlag_value = 0x0000 #means off
#####
        #print "offOnFlag_value.....>>>>", offOnFlag_value

        offOnFlag_hex_value='0x%04X' % (offOnFlag_value)

        offOnBitmap.insert(offOnFlag_length, offOnFlag_hex_value)
        offOnFlag_length = offOnFlag_length + 1
        #offOnFlag_value = 0x0000
    #print "offOnFlag_hex_value is ",offOnBitmap

###to be finished
    #remove valid = N in sheet_IP
    sheet_IP_Valid_length = len(sheet_IP_Valid)-1
    while sheet_IP_Valid_length >= 0:
        if sheet_IP_Valid[sheet_IP_Valid_length] == "N":
            del sheet_IP_RegName[sheet_IP_Valid_length]
            del sheet_IP_Address[sheet_IP_Valid_length]
            del sheet_IP_Mask[sheet_IP_Valid_length]
            del sheet_IP_Invert[sheet_IP_Valid_length]
            del offOnBitmap[sheet_IP_Valid_length]
        sheet_IP_Valid_length = sheet_IP_Valid_length -1
    #print "offOnBitmap again :",offOnBitmap

    #record offOnBitmap that are all OFF or ON value
    sheet_IP_All_off_on_length = len(offOnBitmap)-1
    #print "sheet_IP_All_off_on_length: ", sheet_IP_All_off_on_length
    sheet_IP_All_off_on_counter = 0
    #while sheet_IP_All_off_on_length >= 0:
    while sheet_IP_All_off_on_counter <= sheet_IP_All_off_on_length:
        #print "offOnBitmap[] : ",offOnBitmap[sheet_IP_All_off_on_counter], "sheet_IP_All_off_on_counter :", sheet_IP_All_off_on_counter
        if offOnBitmap[sheet_IP_All_off_on_counter] == "0xFFFF" or offOnBitmap[sheet_IP_All_off_on_counter] == "0x0000":
            #print "sheet_IP_All_off_on_counter in while :", sheet_IP_All_off_on_counter, "pws_init_regName_for_pws_table_index:", pws_init_regName_for_pws_table_index
            #print "sheet_IP_RegName[] :", sheet_IP_RegName
            pws_init_table_IP_RegName.insert(pws_init_regName_for_pws_table_index,sheet_IP_RegName[sheet_IP_All_off_on_counter])
            pws_init_table_IP_Address.insert(pws_init_regName_for_pws_table_index,sheet_IP_Address[sheet_IP_All_off_on_counter])
            pws_init_table_IP_Mask.insert(pws_init_regName_for_pws_table_index,sheet_IP_Mask[sheet_IP_All_off_on_counter])
            pws_init_table_IP_Invert.insert(pws_init_regName_for_pws_table_index,sheet_IP_Invert[sheet_IP_All_off_on_counter])
            pws_init_table_offOnBitmap.insert(pws_init_regName_for_pws_table_index,offOnBitmap[sheet_IP_All_off_on_counter])

            pws_init_regName_for_pws_table_index = pws_init_regName_for_pws_table_index + 1

            #print "all off or on"
        #else :
        #    print "normal"

        sheet_IP_All_off_on_counter = sheet_IP_All_off_on_counter + 1
    #print "pws_init_table_IP_RegName: ", pws_init_table_IP_RegName

    #remove all OFF or ON
    sheet_IP_RegName_length = len(sheet_IP_RegName)-1
    while sheet_IP_RegName_length >= 0:
        #print "offOnBitmap >>>>>>>", offOnBitmap, ", sheet_IP_RegName_length>>", sheet_IP_RegName_length
        if offOnBitmap[sheet_IP_RegName_length] == "0x0000" or  offOnBitmap[sheet_IP_RegName_length] == "0xFFFF":
            del sheet_IP_RegName[sheet_IP_RegName_length]
            del sheet_IP_Address[sheet_IP_RegName_length]
            del sheet_IP_Mask[sheet_IP_RegName_length]
            del sheet_IP_Invert[sheet_IP_RegName_length]
            del offOnBitmap[sheet_IP_RegName_length]
        sheet_IP_RegName_length = sheet_IP_RegName_length -1
    #print "offOnBitmap again >>",offOnBitmap

    #add comment show Block name
    pws_table_file.write('  //'+ x + '\n')

    sheet_IP_RegName_length = len(sheet_IP_RegName) -1
    sheet_IP_index = 0
    while sheet_IP_index <= sheet_IP_RegName_length:
        #write PWS info to file {"regname", address, mask}
        #pws_table_file.write('  {\"' + sheet_IP_RegName[sheet_IP_index]+ '\", ' + sheet_IP_Address[sheet_IP_index] + ', '+ sheet_IP_Mask[sheet_IP_index] + ', '+ offOnBitmap[sheet_IP_index] + '}\n')
        if sheet_IP_Invert[sheet_IP_index] == "N" :
            pws_table_file.write('  {' + sheet_IP_Address[sheet_IP_index] + ', '+ sheet_IP_Mask[sheet_IP_index] + '/*, '+ offOnBitmap[sheet_IP_index] + '*/ /*off=0x01, on=0x00*/, ' + "FALSE" + ', \"' + sheet_IP_RegName[sheet_IP_index]+ '\"},\n')
        else :
            pws_table_file.write('  {' + sheet_IP_Address[sheet_IP_index] + ', '+ sheet_IP_Mask[sheet_IP_index] + '/*, '+ offOnBitmap[sheet_IP_index] + '*/ /*off=0x00, on=0x01*/, ' + "TRUE" + ', \"' +  sheet_IP_RegName[sheet_IP_index]+ '\"},\n')
        #for pws table use include in api layer
        offOnBitmap_for_pws_table.insert(offOnBitmap_regName_for_pws_table_index, offOnBitmap[sheet_IP_index])
        regName_for_pws_table.insert(offOnBitmap_regName_for_pws_table_index, sheet_IP_RegName[sheet_IP_index])
        offOnBitmap_regName_for_pws_table_index = offOnBitmap_regName_for_pws_table_index + 1
        sheet_IP_index = sheet_IP_index +1
    #print "offOnBitmap_for_pws_table", offOnBitmap_for_pws_table
    #print "regName_for_pws_table",regName_for_pws_table
    PWS_Items = PWS_Items + len(sheet_IP_RegName) #1-based

pws_table_file.write('};\n\n')

#write pws init data structure
pws_table_file.write('static const PWS_TABLE_INFO pws_init_setting_info_table[] =\n')
pws_table_file.write('{\n')
flag_index = 0
PWS_init_Items = len(pws_init_table_IP_RegName)
#print "pws init setting info table PWS_init_Items: ", PWS_init_Items
while flag_index <= (PWS_init_Items-1):
    if pws_init_table_IP_Invert[flag_index] == "N" :
        pws_table_file.write('  {' + pws_init_table_IP_Address[flag_index] + ', '+ pws_init_table_IP_Mask[flag_index] + '/*, '+ pws_init_table_offOnBitmap[flag_index] + '*/ /*off=0x01, on=0x00*/, ' + "FALSE" + ', \"' + pws_init_table_IP_RegName[flag_index]+ '\"},\n')
    else :
        pws_table_file.write('  {' + pws_init_table_IP_Address[flag_index] + ', '+ pws_init_table_IP_Mask[flag_index] + '/*, '+ pws_init_table_offOnBitmap[flag_index] + '*/ /*off=0x00, on=0x01*/, ' + "TRUE" + ', \"' + pws_init_table_IP_RegName[flag_index]+ '\"},\n')
    flag_index = flag_index + 1
pws_table_file.write('};\n\n')

#pws_table_file.write('#define PWS_First_Items 0\n')
#pws_table_file.write('#define OutOfRange_PWS_Items ' + str(PWS_Items) + '\n')

#do while loop for every IP list
pws_table_file.write('\n#endif\n')
#close file
#open file (pws_table.h)
pws_table_file = open('mhal_pws_table.h','w')
pws_table_file.write('#ifndef _PWSTABLE_H_\n')
pws_table_file.write('#define _PWSTABLE_H_\n\n')

pws_table_file.write('#define PWS_TBL_VERSION  \"' + pws_table_ver + '\"\n')
pws_table_file.write('#define PWS_First_Items 0\n')
pws_table_file.write('#define OutOfRange_PWS_Items ' + str(PWS_Items) + '\n\n')

#write pws data structure
pws_table_file.write('static const MS_U16 pws_table[] =\n')
pws_table_file.write('{\n')
flag_index = 0
while flag_index <= (PWS_Items-1):
    pws_table_file.write('  ' + offOnBitmap_for_pws_table[flag_index] + ', /*'+ regName_for_pws_table[flag_index] + '*/\n')
    flag_index = flag_index + 1
pws_table_file.write('};\n\n')

#write pws init data structure

pws_table_file.write('#define PWS_Init_First_Items 0\n')
pws_table_file.write('#define OutOfRange_PWS_Init_Items ' + str(len(pws_init_table_IP_RegName)) + '\n\n')

pws_table_file.write('static const MS_U16 pws_init_table[] =\n')
pws_table_file.write('{\n')
flag_index = 0
PWS_init_Items = len(pws_init_table_IP_RegName)
#print "PWS_init_Items: ", PWS_init_Items
while flag_index <= (PWS_init_Items-1):
    pws_table_file.write('  ' + pws_init_table_offOnBitmap[flag_index] + ', /*'+ pws_init_table_IP_RegName[flag_index] + '*/\n')
    flag_index = flag_index + 1
pws_table_file.write('};\n\n')

pws_table_file.write('\n#endif\n')
#print "offOnBitmap_for_pws_table", len(offOnBitmap_for_pws_table)
#print "regName_for_pws_table", len(regName_for_pws_table)
print "Table generating OK...!!!"
