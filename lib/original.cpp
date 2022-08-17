
#include <iostream>
#include "CReedSolomon.h"
#include <string.h>
#include "DES.h"

#define _CRT_SECURE_NO_WARNINGS

////////CRC16

unsigned short CCITT_table[256] = {
    0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50A5, 0x60C6,    0x70E7, 0x8108, 0x9129, 0xA14A, 0xB16B, 0xC18C, 0xD1AD,    0xE1CE, 0xF1EF, 0x1231, 0x0210, 0x3273, 0x2252, 0x52B5,
    0x4294, 0x72F7, 0x62D6, 0x9339, 0x8318, 0xB37B, 0xA35A,    0xD3BD, 0xC39C, 0xF3FF, 0xE3DE, 0x2462, 0x3443, 0x0420,    0x1401, 0x64E6, 0x74C7, 0x44A4, 0x5485, 0xA56A, 0xB54B,
    0x8528, 0x9509, 0xE5EE, 0xF5CF, 0xC5AC, 0xD58D, 0x3653,    0x2672, 0x1611, 0x0630, 0x76D7, 0x66F6, 0x5695, 0x46B4,    0xB75B, 0xA77A, 0x9719, 0x8738, 0xF7DF, 0xE7FE, 0xD79D,
    0xC7BC, 0x48C4, 0x58E5, 0x6886, 0x78A7, 0x0840, 0x1861,    0x2802, 0x3823, 0xC9CC, 0xD9ED, 0xE98E, 0xF9AF, 0x8948,    0x9969, 0xA90A, 0xB92B, 0x5AF5, 0x4AD4, 0x7AB7, 0x6A96,
    0x1A71, 0x0A50, 0x3A33, 0x2A12, 0xDBFD, 0xCBDC, 0xFBBF,    0xEB9E, 0x9B79, 0x8B58, 0xBB3B, 0xAB1A, 0x6CA6, 0x7C87,    0x4CE4, 0x5CC5, 0x2C22, 0x3C03, 0x0C60, 0x1C41, 0xEDAE,
    0xFD8F, 0xCDEC, 0xDDCD, 0xAD2A, 0xBD0B, 0x8D68, 0x9D49,    0x7E97, 0x6EB6, 0x5ED5, 0x4EF4, 0x3E13, 0x2E32, 0x1E51,    0x0E70, 0xFF9F, 0xEFBE, 0xDFDD, 0xCFFC, 0xBF1B, 0xAF3A,
    0x9F59, 0x8F78, 0x9188, 0x81A9, 0xB1CA, 0xA1EB, 0xD10C,    0xC12D, 0xF14E, 0xE16F, 0x1080, 0x00A1, 0x30C2, 0x20E3,    0x5004, 0x4025, 0x7046, 0x6067, 0x83B9, 0x9398, 0xA3FB,
    0xB3DA, 0xC33D, 0xD31C, 0xE37F, 0xF35E, 0x02B1, 0x1290,    0x22F3, 0x32D2, 0x4235, 0x5214, 0x6277, 0x7256, 0xB5EA,    0xA5CB, 0x95A8, 0x8589, 0xF56E, 0xE54F, 0xD52C, 0xC50D,
    0x34E2, 0x24C3, 0x14A0, 0x0481, 0x7466, 0x6447, 0x5424,    0x4405, 0xA7DB, 0xB7FA, 0x8799, 0x97B8, 0xE75F, 0xF77E,    0xC71D, 0xD73C, 0x26D3, 0x36F2, 0x0691, 0x16B0, 0x6657,
    0x7676, 0x4615, 0x5634, 0xD94C, 0xC96D, 0xF90E, 0xE92F,    0x99C8, 0x89E9, 0xB98A, 0xA9AB, 0x5844, 0x4865, 0x7806,    0x6827, 0x18C0, 0x08E1, 0x3882, 0x28A3, 0xCB7D, 0xDB5C,
    0xEB3F, 0xFB1E, 0x8BF9, 0x9BD8, 0xABBB, 0xBB9A, 0x4A75,    0x5A54, 0x6A37, 0x7A16, 0x0AF1, 0x1AD0, 0x2AB3, 0x3A92,    0xFD2E, 0xED0F, 0xDD6C, 0xCD4D, 0xBDAA, 0xAD8B, 0x9DE8,
    0x8DC9, 0x7C26, 0x6C07, 0x5C64, 0x4C45, 0x3CA2, 0x2C83,    0x1CE0, 0x0CC1, 0xEF1F, 0xFF3E, 0xCF5D, 0xDF7C, 0xAF9B,    0xBFBA, 0x8FD9, 0x9FF8, 0x6E17, 0x7E36, 0x4E55, 0x5E74,
    0x2E93, 0x3EB2, 0x0ED1, 0x1EF0 };


unsigned short CCITT_CRC16(unsigned char* data_p, unsigned short len)
{
    unsigned short vcrc;

    vcrc = 0XFFFF;

    for (int i = 0; i < len; i++)
    {
        vcrc = (vcrc << 8) ^ CCITT_table[(vcrc >> 8) ^ data_p[i] & 0x000000FF];
    }

    return vcrc;
}


///////////////

int main()
{
    FILE* fp_input;
    FILE* fp_pn;
    FILE* fp_output;
    FILE* fp_vcdu;
    FILE* fp_sp;
    FILE* fp_tp;
    FILE* fp_pdu;


    unsigned char buf_cadu[1024];
    unsigned char buf_vcdu[886];
    unsigned char pn[1020];
    unsigned char out[1024];
    
    CReedSolomon rs;

    fp_pn = fopen("D:\\lrit\\pn.dat", "rb");
    fp_input = fopen("D:\\lrit\\lrit_img.cadu", "rb");
    fp_output = fopen("D:\\lrit\\lrit_img.cadu.out", "wb");
    fp_vcdu = fopen("D:\\lrit\\vcdu.out", "wb");

    fread(pn, 1, 1020, fp_pn);

    

    while (fread(buf_cadu, 1, 1024, fp_input)) {
        
        //cadu 첫 4byte가 sync니까 그냥 복사
        for (int i = 0; i < 4; i++)
            out[i] = buf_cadu[i];
        //cadu 다음 1020byte(4~1023)는 데이터니까 pn이랑 XOR 시키는 부분
        for (int i = 0; i < 1020; i++)
            out[i + 4] = pn[i] ^ buf_cadu[i + 4];
        
        fwrite(out, 1, 1024, fp_output);
        
        //RS decoding 부분
        rs.init(8, 16, 112, 11, 0, 4, 0, 1);
        rs.Decode(out + 4);
        std::cout << rs.CorrectableErrorsInFrame() << "byte error corrected!" << std::endl;

        //vcdu 정리하는 부분
        if (out[5] == 0xC0) // 0xC0 == 192, VCID=0인 것만 고르기
        {
            for (int i = 0; i < 886; i++)
            {
                buf_vcdu[i] = out[i + 10];
            }
            fwrite(buf_vcdu, 1, 886, fp_vcdu);
        }

    }
    
    fclose(fp_input);
    fclose(fp_pn);
    fclose(fp_output);
    fclose(fp_vcdu);
    

    char file_name[100] = { 0 };
    unsigned char buf_sp[886];

    fp_vcdu = fopen("D:\\lrit\\vcdu.out", "rb");
    int file_num = 0;

    const char* file_path = "D:\\lrit";


    while (fread(buf_sp, 1, 886, fp_vcdu)) {
        memset(file_name, 0, sizeof(file_name));//filename initialize to 0
        int header_point = buf_sp[0] * 256 + buf_sp[1];
        unsigned char buf_tobesaved[884];

        if (header_point == 0x0000) {

            for (int i = 0; i < 884; i++)
                buf_tobesaved[i] = buf_sp[i + 2];
            sprintf(file_name, "%s\\source_packet\\sp.out%d", file_path, file_num++);
            fp_sp = fopen(file_name, "wb");
            fwrite(buf_tobesaved, 1, 884, fp_sp);
        }

        else if (0 < header_point && header_point < 0x07ff) {

            for (int i = 0; i < header_point; i++)
                buf_tobesaved[i] = buf_sp[i + 2];
            fwrite(buf_tobesaved, 1, header_point, fp_sp);
            fclose(fp_sp);

            sprintf(file_name, "%s\\source_packet\\sp.out%d", file_path, file_num++);
            fp_sp = fopen(file_name, "wb");

            for (int i = 0; i < 884 - header_point; i++)
                buf_tobesaved[i] = buf_sp[i + 2 + header_point];

            fwrite(buf_tobesaved, 1, 884 - header_point, fp_sp);
        }

        else if (header_point == 0x07ff) {
            for (int i = 0; i < 884; i++)
                buf_tobesaved[i] = buf_sp[i + 2];

            fwrite(buf_tobesaved, 1, 884, fp_sp);
        }

        else {
            std::cout << "Error occured ( Header Point not in range [0,2047] )" << std::endl;
            fclose(fp_sp);
            break;
        }


    }
    fclose(fp_vcdu);

    
    int tp_num = 0;
    
    for (int i = 0; i < 123; i++) {
        int df_len = 0;
        char sp_file_name[100] = { 0 };
        char tp_file_name[100] = { 0 };
        unsigned char buf_tp[8198] = { 0 };
        
        sprintf(sp_file_name, "%s\\source_packet\\sp.out%d", file_path, i);
        sprintf(tp_file_name, "%s\\tp_file\\tp.out%d", file_path, tp_num);

        fp_sp = fopen(sp_file_name, "rb");
        fread(buf_tp, 1, sizeof(buf_tp), fp_sp);
        df_len = buf_tp[4] * 256 + buf_tp[5] - 1;//data field length=packet length-1


        /////if seq flag==01
        if (buf_tp[2] >= 0b01000000 && buf_tp[2] < 0b10000000) {
            fp_tp = fopen(tp_file_name, "wb");

            fseek(fp_sp, 6, SEEK_SET);//sp의 파일 포인터를 파일 처음에서 6바이트만큼 순방향 이동
            fread(buf_tp, df_len, 1, fp_sp);//dflen 길이만큼 읽어서 buftp에 저장
            fwrite(buf_tp, df_len, 1, fp_tp);//dflen 길이만큼 fptp에 저장
        }
        /////if seq flag==00 and packet length != 0
        else if (buf_tp[2] < 0b01000000 && buf_tp[4] * 256 + buf_tp[5] != 0) {
            fseek(fp_sp, 6, SEEK_SET);
            fread(buf_tp, df_len, 1, fp_sp);
            fwrite(buf_tp, df_len, 1, fp_tp);

        }
        /////if seq flag==10
        else if (buf_tp[2] >= 0b10000000) {
            fseek(fp_sp, 6, SEEK_SET);
            fread(buf_tp, df_len, 1, fp_sp);
            fwrite(buf_tp, df_len, 1, fp_tp);
            fclose(fp_tp);
            tp_num++;//next tp
        }

        /////// if empty packet
        else if (buf_tp[4] * 256 + buf_tp[5] == 0) {
            continue;//do nothing
        }

        else {
            std::cout << "tp error!" << std::endl;
            fclose(fp_sp);
            break;
        }

        //std::cout << sizeof(buf_tp) << std::endl;
        fclose(fp_sp);


        //CRC16 check
        unsigned char buf_crc[8198] = { 0 };
        fp_sp = fopen(sp_file_name, "rb");

        fread(buf_crc, 6, 1, fp_sp);//dflen 길이만큼 읽어서 buftp에 저장

        if (buf_crc[4] * 256 + buf_crc[5] != 0) {
            int crc_check = 5;
            fseek(fp_sp, 6, SEEK_SET);//sp의 파일 포인터를 파일 처음에서 6바이트만큼 순방향 이동
            fread(buf_crc, df_len + 2, 1, fp_sp);//dflen 길이만큼 읽어서 buftp에 저장

            crc_check = CCITT_CRC16(buf_crc, df_len);

            if (crc_check == buf_crc[df_len] * 256 + buf_crc[df_len + 1]) 
                std::cout << "source packet #" << i << " CRC OK" << std::endl;
            else
                std::cout << "source packet #" << i << " CRC ERROR!!!" << std::endl;
        }
        fclose(fp_sp);
    }



    //////tp to s_pdu

    const char* tp_file_path = "D:\\lrit\\tp_file";
    const char* pdu_file_path = "D:\\lrit\\pdu_file";

    char tp_file_name[100] = { 0 };
    char pdu_file_name[100] = { 0 };

    tp_num = 0;
    int pdu_num = 0;



    unsigned char* buf;


    for (int i = 0; i < 10; i++) {

        sprintf(tp_file_name, "%s\\tp.out%d", tp_file_path, tp_num++);
        sprintf(pdu_file_name, "%s\\pdu.out%d", pdu_file_path, pdu_num++);
        fp_tp = fopen(tp_file_name, "rb");
        fp_pdu = fopen(pdu_file_name, "wb");
        fseek(fp_tp, 0, SEEK_END);
        int size = ftell(fp_tp);
        fseek(fp_tp, 3910, SEEK_SET);//

        buf = (unsigned char*)malloc(size - 3910);//
        memset(buf, 0, size - 3910);//

        fread(buf, size - 3910, 1, fp_tp);//
        fwrite(buf, size - 3910, 1, fp_pdu);//10+3900
        fclose(fp_pdu);
        fclose(fp_tp);
    }
    fclose(fp_pdu);


    //암호 해제
    FILE* pf_key;
    FILE* pf_jpg;
    FILE* full_disk;

    unsigned long long key = 0x3B0779919DC237A4;
    memset(pdu_file_name, 0, 100);
    char jpg_file_name[100] = { 0 };

    DES des;
    des.setKey(key);

    for (int i = 0; i < 10; i++) {
        unsigned char buf_key[134544];

        sprintf(pdu_file_name, "D:\\lrit\\pdu_file\\pdu.out%d", i);
        pf_key = fopen(pdu_file_name, "rb");
        fseek(pf_key, 0, SEEK_END);
        long file_size = ftell(pf_key);
        fseek(pf_key, 0, SEEK_SET);

        fread(buf_key, file_size, 1, pf_key);

        des.decrypt(buf_key, file_size);

        sprintf(jpg_file_name, "D:\\lrit\\picture\\pic%d.jpg", i);
        pf_jpg = fopen(jpg_file_name, "wb");
        fwrite(buf_key, sizeof(buf_key), 1, pf_jpg);
        
        fclose(pf_key);
        fclose(pf_jpg);
    }

    return 0;
}

