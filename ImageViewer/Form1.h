#pragma once

#include <iostream>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <fstream>
#include <cmath>
#include <windows.h>
#include "font.h"
#include "font816.h"
#include "msgothic816.h"
#include "1123Font.h"
#include "icon1616.h"
#include "gulimBold13pt.h"

using namespace std;


#include "xPng.h"
#include "xInflate.h"
#include "xBitmap.h"


#define DQT 	 0xDB	// Define Quantization Table
#define SOF 	 0xC0	// Start of Frame (size information)
#define DHT 	 0xC4	// Huffman Table
#define SOI 	 0xD8	// Start of Image
#define SOS 	 0xDA	// Start of Scan
#define EOI 	 0xD9	// End of Image, or End of File
#define APP0	 0xE0

#define HUFFMAN_TABLES		2
#define COMPONENTS			4

#define cY	1
#define cCb	2
#define cCr	3

//----------------------------------------------------------------------------------------------------------------


#define BYTE_TO_CHAR(x) ( (x)[0])
#define BYTE_TO_WORD(x) ( ((x)[0]<<8)|(x)[1])
#define BYTE_TO_INT(x)  ( ((x)[0]<<24) | ((x)[1]<<16) | ((x)[2] << 8) | ((x)[3]) << 0) 

#define XPNG_ABS(x)						(x)< 0 ?  -(x) : (x)


#define IHDR (0x49 << 24) | (0x48 << 16) | (0x44 << 8) | (0x52 << 0)
#define PLTE (0x50 << 24) | (0x4C << 16) | (0x54 << 8) | (0x45 << 0) 
#define IDAT (0x49 << 24) | (0x44 << 16) | (0x41 << 8) | (0x54 << 0)
#define IEND (0x49 << 24) | (0x45 << 16) | (0x4E << 8) | (0x44 << 0)

#define tIME (0x74 << 24) | (0x49 << 16) | (0x4d << 8) | (0x45 << 0)
#define pHYs (0x70 << 24) | (0x48 << 16) | (0x59 << 8) | (0x73 << 0)
#define gAMA (0x67 << 24) | (0x41 << 16) | (0x4d << 8) | (0x41 << 0)
#define tEXt (0x74 << 24) | (0x45 << 16) | (0x58 << 8) | (0x74 << 0)
#define sRGB (0x73 << 24) | (0x52 << 16) | (0x47 << 8) | (0x42 << 0)

// RGB BGR 변환 테이블
static const int	colorMapTable_RGBA_2_BGRA[] = {2, 0, -2, 0};
static const int	colorMapTable_RGB_2_BGR[] = {2, 0, -2};

////////////////////////////////////////////////
//
// 중복 되는 부분 - 속도를 위해서 매크로로 처리
//
#define MOVETONEXTBUFFER													\
	imageCurBuffer++;													\
	imageCurWidthByte--;												\
	ColorMapTablePos = (ColorMapTablePos+1) % ColorMapTableLen;			\
	\
	outBuf++;															\
	outBufLen--;														\
	\
	if(imageCurWidthByte==0)											\
{																	\
	imageCurBuffer += imagePaddingByte;								\
	imageCurWidthByte = imageWidthByte;								\
	break;															\
}

#define MOVETONEXTBUFFER_4_PAL												\
	imageCurBuffer+=4;													\
	imageCurWidthByte-=4;												\
	\
	outBuf++;															\
	outBufLen--;														\
	\
	if(imageCurWidthByte==0)											\
{																	\
	imageCurBuffer += imagePaddingByte;								\
	imageCurWidthByte = imageWidthByte;								\
	break;															\
}

#define MOVETONEXTBUFFER_3_PAL												\
	imageCurBuffer+=3;													\
	imageCurWidthByte-=4;												\
	\
	outBuf++;															\
	outBufLen--;														\
	\
	if(imageCurWidthByte==0)											\
{																	\
	imageCurBuffer += imagePaddingByte;								\
	imageCurWidthByte = imageWidthByte;								\
	break;															\
}


// 테이블 처리를 하지 않는다.
#define MOVETONEXTBUFFER_FAST(len)											\
	imageCurWidthByte-=len;												\
	outBufLen-=len;														\
	outBuf +=len ;														\
	imageCurBuffer += len;


#define	PIXELFORMAT_24  24
#define	PIXELFORMAT_32  32

//------------------------------------------------------------------------------------------------

static double cos1[8][8] = {
	{1.0000, 0.9808, 0.9240, 0.8316, 0.7074, 0.5560, 0.3832, 0.1958},
	{1.0000, 0.8316, 0.3832, -0.1942, -0.7063, -0.9805, -0.9246, -0.5573},
	{1.0000, 0.5560, -0.3818, -0.9805, -0.7085, 0.1926, 0.9227, 0.8334},
	{1.0000, 0.1958, -0.9233, -0.5573, 0.7051, 0.8334, -0.3788, -0.9817},
	{1.0000, -0.1942, -0.9246, 0.5533, 0.7096, -0.8290, -0.3876, 0.9795},
	{1.0000, -0.5547, -0.3847, 0.9814, -0.7040, -0.2005, 0.9264, -0.8272},
	{1.0000, -0.8307, 0.3803, 0.1989, -0.7108, 0.9820, -0.9209, 0.5480},
	{1.0000, -0.9805, 0.9227, -0.8290, 0.7029, -0.5493, 0.3744, -0.1848}
};
static int ZigZagArray[64] = 
{
	0,   1,   5,  6,   14,  15,  27,  28,
	2,   4,   7,  13,  16,  26,  29,  42,
	3,   8,  12,  17,  25,  30,  41,  43,
	9,   11, 18,  24,  31,  40,  44,  53,
	10,  19, 23,  32,  39,  45,  52,  54,
	20,  22, 33,  38,  46,  51,  55,  60,
	21,  34, 37,  47,  50,  56,  59,  61,
	35,  36, 48,  49,  57,  58,  62,  63,
};

/***************************************************************************/

#pragma pack(1) // 데이터를 1byte 씩 처리 하라는 전처리..구조체가 끝날때는 다시 닫아줘야지
struct stBMFH // BitmapFileHeader & BitmapInfoHeader
{
	// BitmapFileHeader
	char         bmtype[2];     // 2 bytes - 'B' 'M'
	int iFileSize;     // 4 bytes
	short int    reserved1;     // 2 bytes
	short int    reserved2;     // 2 bytes
	int iOffsetBits;   // 4 bytes
	// End of stBMFH structure - size of 14 bytes
	// BitmapInfoHeader
	int iSizeHeader;    // 4 bytes - 40
	int iWidth;         // 4 bytes
	int iHeight;        // 4 bytes
	short int    iPlanes;        // 2 bytes
	short int    iBitCount;      // 2 bytes
	int Compression;    // 4 bytes
	int iSizeImage;     // 4 bytes
	int iXPelsPerMeter; // 4 bytes
	int iYPelsPerMeter; // 4 bytes
	int iClrUsed;       // 4 bytes
	int iClrImportant;  // 4 bytes
	// End of stBMIF structure - size 40 bytes
	// Total size - 54 bytes
};
#pragma pack()

struct stBlock
{
	int value;					// Decodes to.
	int length;				// Length in bits.
	unsigned short int code;	// 2 byte code (variable length)
};

/***************************************************************************/

// AC HuffmanTable
struct stHuffmanTable
{
	unsigned char	m_length[17];		// 17 values from jpg file, 
	// k =1-16 ; L[k] indicates the number of Huffman codes of length k
	unsigned char	m_hufVal[257];		// 256 codes read in from the jpeg file

	int				m_numBlocks;
	stBlock			m_blocks[256];
};
// DC HuffmanTable
struct stHuffmanTable2
{
	unsigned char	m_length[17];		// 17 values from jpg file, 
	// k =1-16 ; L[k] indicates the number of Huffman codes of length k
	unsigned char	m_hufVal[257];		// 256 codes read in from the jpeg file

	int				m_numBlocks;
	struct stBlock			m_blocks[16];
};


struct stComponent 
{
	unsigned int			m_hFactor;
	unsigned int			m_vFactor;
	float *				m_qTable;			// Pointer to the quantisation table to use

	stHuffmanTable*		m_acTable;
	stHuffmanTable2*		m_dcTable;

	short int				m_DCT[65];			// DCT coef
	int					m_previousDC;
};



struct stJpegData
{
	unsigned char*		m_rgb;				// Final Red Green Blue pixel data
	unsigned int		m_width;			// Width of image
	unsigned int		m_height;			// Height of image

	const unsigned char*m_stream;			// Pointer to the current stream

	stComponent			m_component_info[COMPONENTS];

	float				m_Q_tables[COMPONENTS][64];	// quantization tables

	stHuffmanTable2		m_HTDC[HUFFMAN_TABLES];		// DC huffman tables  
	stHuffmanTable		m_HTAC[HUFFMAN_TABLES];		// AC huffman tables

	// Temp space used after the IDCT to store each components
	unsigned char		m_Y[64*4];
	unsigned char		m_Cr[64];
	unsigned char		m_Cb[64];

	// 블록그리기 에는 필요없음
	unsigned char *		m_colourspace;
};



namespace ImageViewer {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	FILE *fp;
	string pat;

	typedef struct 
	{
		int   bfSize;
		short    bfReserved1;
		short    bfReserved2;
		int  bfOffBits;
	} BitMapHeader;

	typedef struct
	{
		int    biSize;
		int    biWidth;
		int    biHeight;
		short  biPlanes;
		short  biBitCount;
		int    biCompression;
		int    biSizeImage;
		int    biXPelsPerMeter;
		int    biYPelsPerMeter;
		int    biClrUsed;
		int    biClrImportant;
	} BitMapImgHeader;

	/// <summary>
	/// Form1에 대한 요약입니다.
	///
	/// 경고: 이 클래스의 이름을 변경하면 이 클래스가 의존하는
	///          모든 .resx 파일과 관련된 관리되는 리소스 컴파일러 도구의
	///          '리소스 파일 이름' 속성도 변경해야 합니다. 그렇지 않은 경우
	///          디자이너는 이 폼과 관련된 지역화된 리소스와
	///          올바르게 상호 작용할 수 없습니다.
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(void)
		{
			InitializeComponent();
			//
			//TODO: 생성자 코드를 여기에 추가합니다.
			//
		}

	protected:
		/// <summary>
		/// 사용 중인 모든 리소스를 정리합니다.
		/// </summary>
		~Form1()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^  openButton;
	private : System::Windows::Forms::Panel^ imgPanel;
	private: System::Windows::Forms::PictureBox^  pictureBox1;

	private: System::Windows::Forms::Button^  jpgButton;
	private: System::Windows::Forms::Button^  button1;
	private: System::Windows::Forms::Button^  pngBtn;

	private: int pixX;
	private: int pixY;

	private: unsigned int g_reservoir;
	private: unsigned int g_nbits_in_reservoir;
	private: System::Windows::Forms::TextBox^  fontData;

	private: System::Windows::Forms::Button^  drawText;



	protected: 

	private:
		/// <summary>
		/// 필수 디자이너 변수입니다.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// 디자이너 지원에 필요한 메서드입니다.
		/// 이 메서드의 내용을 코드 편집기로 수정하지 마십시오.
		/// </summary>
		void InitializeComponent(void)
		{
			this->openButton = (gcnew System::Windows::Forms::Button());
			this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
			this->jpgButton = (gcnew System::Windows::Forms::Button());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->pngBtn = (gcnew System::Windows::Forms::Button());
			this->fontData = (gcnew System::Windows::Forms::TextBox());
			this->drawText = (gcnew System::Windows::Forms::Button());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox1))->BeginInit();
			this->SuspendLayout();
			// 
			// openButton
			// 
			this->openButton->Location = System::Drawing::Point(772, 12);
			this->openButton->Name = L"openButton";
			this->openButton->Size = System::Drawing::Size(127, 23);
			this->openButton->TabIndex = 0;
			this->openButton->Text = L"bmp load";
			this->openButton->UseVisualStyleBackColor = true;
			this->openButton->Click += gcnew System::EventHandler(this, &Form1::openButton_Click);
			// 
			// pictureBox1
			// 
			this->pictureBox1->BackColor = System::Drawing::Color::Black;
			this->pictureBox1->Location = System::Drawing::Point(12, 12);
			this->pictureBox1->Name = L"pictureBox1";
			this->pictureBox1->Size = System::Drawing::Size(320, 240);
			this->pictureBox1->TabIndex = 1;
			this->pictureBox1->TabStop = false;
			// 
			// jpgButton
			// 
			this->jpgButton->Location = System::Drawing::Point(772, 41);
			this->jpgButton->Name = L"jpgButton";
			this->jpgButton->Size = System::Drawing::Size(127, 23);
			this->jpgButton->TabIndex = 3;
			this->jpgButton->Text = L"jpg load";
			this->jpgButton->UseVisualStyleBackColor = true;
			this->jpgButton->Click += gcnew System::EventHandler(this, &Form1::jpgButton_Click);
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(772, 70);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(127, 21);
			this->button1->TabIndex = 4;
			this->button1->Text = L"jpg 1px load";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &Form1::button1_Click);
			// 
			// pngBtn
			// 
			this->pngBtn->Location = System::Drawing::Point(772, 97);
			this->pngBtn->Name = L"pngBtn";
			this->pngBtn->Size = System::Drawing::Size(127, 21);
			this->pngBtn->TabIndex = 5;
			this->pngBtn->Text = L"png load";
			this->pngBtn->UseVisualStyleBackColor = true;
			this->pngBtn->Click += gcnew System::EventHandler(this, &Form1::pngBtn_Click);
			// 
			// fontData
			// 
			this->fontData->Location = System::Drawing::Point(772, 149);
			this->fontData->Name = L"fontData";
			this->fontData->Size = System::Drawing::Size(127, 21);
			this->fontData->TabIndex = 6;
			this->fontData->Text = L"M";
			// 
			// drawText
			// 
			this->drawText->Location = System::Drawing::Point(824, 176);
			this->drawText->Name = L"drawText";
			this->drawText->Size = System::Drawing::Size(75, 23);
			this->drawText->TabIndex = 7;
			this->drawText->Text = L"draw";
			this->drawText->UseVisualStyleBackColor = true;
			this->drawText->Click += gcnew System::EventHandler(this, &Form1::drawText_Click);
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(7, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(911, 445);
			this->Controls->Add(this->drawText);
			this->Controls->Add(this->fontData);
			this->Controls->Add(this->pngBtn);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->jpgButton);
			this->Controls->Add(this->pictureBox1);
			this->Controls->Add(this->openButton);
			this->Name = L"Form1";
			this->Text = L"Form1";
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox1))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
		// bmp 버튼
	private: System::Void openButton_Click(System::Object^  sender, System::EventArgs^  e) 
			 {
				 // Displays an OpenFileDialog so the user can select a Cursor.
				 OpenFileDialog ^ openFileDialog1 = gcnew OpenFileDialog();
				 openFileDialog1->Filter = "Bitmap Files|*.bmp";
				 openFileDialog1->Title = "Select a Bitmap File";

				 // Show the Dialog.
				 // If the user clicked OK in the dialog and
				 // a .CUR file was selected, open it.
				 if (openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK)
				 {
					 String^ path = openFileDialog1->FileName;

					 pat = toStandardString(path);

					 DrawBMPPixel2();

				 }

			 }
			 // 사용안함
			 void DrawBMPPixel()
			 {
				 // 버퍼만들기
				 unsigned char *buffer = (unsigned char *)malloc(4096);
				 //byte buffer[8096];

				 // 파일열기
				 FILE *fp = fopen(pat.c_str(), "rb");    

				 if(fp == NULL)
				 {
					 Windows::Forms::MessageBox::Show("파일이상");
					 return;
				 }

				 // seek를 마지막으로 옮겨서 파일 확인
				 fseek(fp, 0, SEEK_END);
				 // 파일사이즈
				 int fileLength = ftell(fp);

				 // 다시 seek를 처음으로 옮김
				 fseek(fp, 0, SEEK_SET);


				 // 비트맵헤더 구조체
				 BitMapHeader bmh;

				 // BM Check
				 char bfType[2];

				 fread(bfType, 2, 1, fp);

				 if(bfType[0] != 'B' || bfType[1] != 'M')
				 {
					 return;
				 }

				 fread(&bmh, sizeof(BitMapHeader), 1, fp);
				 // 파일을 read해서 buffer에 저장(버퍼, 데이터하나크기, 데이터갯수, 파일포인터)
				 //fread(buffer,54,1, fp);

				 // 비트맵정보헤더 구조체
				 BitMapImgHeader bmih;

				 fread(&bmih, sizeof(BitMapImgHeader), 1, fp);


				 Bitmap ^ bmp = gcnew Bitmap(bmih.biWidth, bmih.biHeight);
				 pictureBox1->Width::set(bmih.biWidth);
				 pictureBox1->Height::set(bmih.biHeight);

				 int red;
				 int green;
				 int blue;

				 int zeroPadding = 0;

				 // 4바이트씩 읽는데 남은바이트가 1개라면
				 // 4바이트 할당 후 00을 3개 붙이고
				 // 남은바이트가 3개면, 00을 1개 붙여서 빈데이터가 없도록 맞춤
				 // zeroPadding은 가로한줄을 읽고 붙음(나중에 가로 한 줄을 읽을 때 필요)
				 if((4 - (bmih.biWidth * 3) % 4) == 1)
					 zeroPadding = 1;
				 else if((4 -(bmih.biWidth * 3) % 4) == 2)
					 zeroPadding = 2;
				 else if((4 -(bmih.biWidth * 3) % 4) == 3)
					 zeroPadding = 3;

				 // color buffer 초기화

				 unsigned char *colorBuffer = (unsigned char *)malloc(1024);
				 //byte colorBuffer[1024];

				 //// 256컬러 파일이면? color buffer에 컬러테이블을 넣음
				 //if(bmih.biBitCount == 8)
				 //{
				 // fread(colorBuffer, 1024, 1, fp);
				 //}

				 // seek를 offset점으로 옮김
				 fseek(fp, bmh.bfOffBits, SEEK_SET);


				 // 세로그리기
				 for(int i=0;i<bmih.biHeight;i++)
				 {
					 // 256이라면...
					 if(bmih.biBitCount == 8)
					 {
						 // width + 0 붙은것만큼 읽어서 buffer에 넣음
						 fread(buffer, bmih.biWidth + zeroPadding,1, fp);
					 }
					 // 24비트면?
					 else
					 {
						 // BGR이 각 1바이트씩 저장되니까 너비 * 3 이 가로한줄 데이터길이 + 00 이 붙은 갯수만큼 읽음
						 fread(buffer, bmih.biWidth*3 + zeroPadding,1, fp);
					 }
					 // 버퍼의 옵셋 초기화(가로 한줄 다 읽으면 다음 가로 한줄을 읽기때문에 offset을 0으로 초기화)
					 int pixelOffset = 0;

					 // 가로그리기
					 for(int j=0;j<bmih.biWidth;j++)
					 {
						 // 픽셀데이터의 순서가 BGR이기 때문에 blue, green, red 순서(offset은 각 1씩 증가)
						 blue = pixelOffset;
						 green = pixelOffset+1;
						 red = pixelOffset+2;

						 //// 256색이면?
						 //if(bmih.biBitCount == 8)
						 //{
						 // // 하나의 pixel의 color값은 color table에 들어있다
						 // // pixel data의 0번째부터 찾음
						 // // 256색의 color Table은 4바이트 안에 B G R 0 순으로 들어가있음
						 // // pixel data의 값은 color Table의 index 에 들어가서 찾아야 하기때문에
						 // // buffer[offset] * 4(B G R 0)
						 // // 그러므로 col은 pixel의 BLUE VALUE를 가지고 있다
						 // int col = buffer[pixelOffset]*4;

						 // // color table에서 색상정보를 가져온다(BGR -> RGB)
						 // Color rColor = Color::FromArgb(colorBuffer[col+2], colorBuffer[col+1], colorBuffer[col]);

						 // bmp->SetPixel(j, bmih.biHeight-i-1, rColor);

						 // pixelOffset += 1;
						 //}
						 //else
						 {
							 // 버퍼의 Red값, Green값, Blue값으로 색을 만들자
							 Color rColor = Color::FromArgb(buffer[red], buffer[green], buffer[blue]);

							 // 픽셀그림
							 bmp->SetPixel(j, bmih.biHeight-i-1, rColor);

							 // offset은 bgr 씩 오르기때문에 3씩 증가시킴
							 pixelOffset += 3;
						 }

					 }
				 }

				 pictureBox1->Image = bmp;

				 free(colorBuffer);
				 free(buffer);


			 }
			 //

			 // 비트맵파일 그리기
			 void DrawBMPPixel2()

			 {
				 // 버퍼만들기
				 unsigned char *buffer = (unsigned char *)malloc(4096);
				 // 파일열기
				 FILE *fp = fopen(pat.c_str(), "rb");    

				 if(fp == NULL)
				 {
					 Windows::Forms::MessageBox::Show("파일이상");
					 return;
				 }

				 // seek를 마지막으로 옮겨서 파일 확인
				 fseek(fp, 0, SEEK_END);
				 // 파일사이즈
				 int fileLength = ftell(fp);

				 // 다시 seek를 처음으로 옮김
				 fseek(fp, 0, SEEK_SET);


				 // 비트맵헤더 구조체
				 BitMapHeader bmh;

				 // BM Check
				 char bfType[2];

				 fread(bfType, 2, 1, fp);

				 if(bfType[0] != 'B' || bfType[1] != 'M')
				 {
					 return;
				 }

				 fread(&bmh, sizeof(BitMapHeader), 1, fp);

				 // 비트맵정보헤더 구조체
				 BitMapImgHeader bmih;

				 fread(&bmih, sizeof(BitMapImgHeader), 1, fp);


				 Bitmap ^ bmp = gcnew Bitmap(bmih.biWidth, bmih.biHeight);
				 pictureBox1->Width::set(bmih.biWidth);
				 pictureBox1->Height::set(bmih.biHeight);

				 int zeroPadding = 0;

				 // 4바이트씩 읽는데 남은바이트가 1개라면
				 // 4바이트 할당 후 00을 3개 붙이고
				 // 남은바이트가 3개면, 00을 1개 붙여서 빈데이터가 없도록 맞춤
				 // zeroPadding은 가로한줄을 읽고 붙음(나중에 가로 한 줄을 읽을 때 필요)
				 if((4 - (bmih.biWidth * 3) % 4) == 1)
					 zeroPadding = 1;
				 else if((4 -(bmih.biWidth * 3) % 4) == 2)
					 zeroPadding = 2;
				 else if((4 -(bmih.biWidth * 3) % 4) == 3)
					 zeroPadding = 3;

				 // seek를 offset점으로 옮김
				 fseek(fp, bmh.bfOffBits, SEEK_SET);

				 // 세로그리기
				 for(int i=0;i<bmih.biHeight;i++)
				 {
					 // BGR이 각 1바이트씩 저장되니까 너비 * 3 이 가로한줄 데이터길이 + 00 이 붙은 갯수만큼 읽음
					 fread(buffer, bmih.biWidth*3 + zeroPadding,1, fp);
					 // 버퍼의 옵셋 초기화(가로 한줄 다 읽으면 다음 가로 한줄을 읽기때문에 offset을 0으로 초기화)
					 int pixelOffset = 0;

					 // 가로그리기
					 for(int j=0;j<bmih.biWidth;j++)
					 {
						 RenderWidthPixel(bmp, buffer, pixelOffset, j, i, bmih.biHeight);

						 pixelOffset += 3;

					 }
				 }

				 pictureBox1->Image = bmp;

				 free(buffer);

			 }
			 void RenderWidthPixel(Bitmap ^bmp, unsigned char *buffer, int offset, int widthCount, int heightCount, int imageHeight)
			 {
				 // 버퍼의 Red값, Green값, Blue값으로 색을 만들자
				 Color rColor = Color::FromArgb(buffer[offset+2], buffer[offset+1], buffer[offset]);

				 // 픽셀그림
				 bmp->SetPixel(widthCount, imageHeight-heightCount-1, rColor);

			 }


			 // jpg 버튼
	private: System::Void jpgButton_Click(System::Object^  sender, System::EventArgs^  e) 
			 {
				 // Displays an OpenFileDialog so the user can select a Cursor.
				 OpenFileDialog ^ openFileDialog1 = gcnew OpenFileDialog();
				 openFileDialog1->Filter = "JPEG Files|*.jpg";
				 openFileDialog1->Title = "Select a JPEG File";

				 // Show the Dialog.
				 // If the user clicked OK in the dialog and
				 // a .CUR file was selected, open it.
				 if (openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK)
				 {
					 String^ path = openFileDialog1->FileName;

					 pat = toStandardString(path);

					 // 다시불러올 경우 초기화
					 g_reservoir = 0;
					 g_nbits_in_reservoir = 0;

					 DrawJPGPixel();
				 }

			 }
			 void DrawJPGPixel()
			 {
				 if(!JPGParser())
				 {
					 Windows::Forms::MessageBox::Show("Cannot Load JPEG...\n");
				 }
			 }
			 inline bool JPGParser()
			 {
				 FILE *fp;
				 unsigned int lengthOfFile;
				 unsigned char *buf;


				 // Load the Jpeg into memory
				 fp = fopen(pat.c_str(), "rb");

				 lengthOfFile = FileSize(fp);
				 buf = new unsigned char[lengthOfFile + 4];

				 fread(buf, lengthOfFile, 1, fp);
				 fclose(fp);

				 unsigned char* rgbpix = NULL;
				 unsigned int width  = 0;
				 unsigned int height = 0;



				 if(DecodeJpgFileData(buf, lengthOfFile, &rgbpix, &width, &height))
				 {
					 if (rgbpix==NULL)
					 {
						 Windows::Forms::MessageBox::Show("Failed to decode jpg\n");

						 delete[] buf;
						 //delete[] rgbpix;
					 }
					 else
					 {
						 // 1. 파일로 만든다
						 //WriteBMP24("c:\\toBmp\\output.bmp", width, height, rgbpix);

						 // 2. 바로 그린다
						 DrawJPEGData(width, height, rgbpix, 3);

						 delete[] buf;
						 //delete[] rgbpix;

						 return true;
					 }
				 }
				 return false;

			 }
			 // jpg 한번에 그리기
			 void DrawJPEGData(int width, int height, unsigned char* RGB, int isrgba)
			 {

				 int iNumPaddedBytes = 4 - (width * isrgba) % 4;
				 iNumPaddedBytes = iNumPaddedBytes % 4;

				 Bitmap ^ bmp = gcnew Bitmap(width, height);
				 pictureBox1->Width::set(width);
				 pictureBox1->Height::set(height);

				 unsigned char *rgbData = (unsigned char *)malloc(width*height*isrgba);

				 int offset = 0;
				 // 거꾸로 저장되어있는 Pixel 값을 역으로 저장
				 for (int y=height-1; y>=0; y--)
				 {
					 for (int x=0; x<width; x++)
					 {
						 int i = (x + (width)*y) * isrgba;
						 unsigned int rgbpix = (RGB[i]<<16)|(RGB[i+1]<<8)|(RGB[i+2]<<0);
						 memcpy(rgbData + offset, &rgbpix, isrgba);

						 offset += isrgba;
					 }
					 if (iNumPaddedBytes>0)
					 {
						 unsigned char pad = 0x00;
						 memcpy(rgbData + offset, &pad, iNumPaddedBytes);
						 offset += iNumPaddedBytes;

					 }
				 }

				 // 세로그리기
				 int arrayOffset = 0;
				 for(int i=0;i<height;i++)
				 {
					 // 버퍼의 옵셋 초기화(가로 한줄 다 읽으면 다음 가로 한줄을 읽기때문에 offset을 0으로 초기화)
					 int pixelOffset = 0;

					 unsigned char *widthData = (unsigned char *)malloc(width*isrgba);


					 memcpy(widthData, rgbData + arrayOffset, width*isrgba);

					 // 가로그리기
					 for(int j=0;j<width;j++)
					 {
						 RenderWidthPixel(bmp, widthData, pixelOffset, j, i, height);

						 pixelOffset += isrgba;
						 arrayOffset += isrgba;	

					 }

					 free(widthData);
				 }

				 pictureBox1->Image = bmp;


				 free(rgbData);

			 }
			 bool DecodeJpgFileData(const unsigned char* buf, const int sizeBuf, unsigned char** rgbpix, unsigned int* width, unsigned int* height)
			 {
				 stJpegData* jdec = new stJpegData();

				 if (!JpegParseHeader(jdec, buf, sizeBuf))
				 {
					 Windows::Forms::MessageBox::Show("ERROR > parsing jpg header\n");
					 return false;
				 }

				 JpegDecode(jdec);

				 JpegGetImageSize(jdec, width, height);

				 *rgbpix = jdec->m_rgb;

				 //delete[] jdec->m_rgb;

				 delete jdec;

				 return true;
			 }
			 inline bool JpegDecode(stJpegData *jdata)
			 {
				 int hFactor = jdata->m_component_info[cY].m_hFactor;
				 int vFactor = jdata->m_component_info[cY].m_vFactor;

				 // RGB24:
				 if (jdata->m_rgb == NULL)
				 {
					 int h = jdata->m_height*3;
					 int w = jdata->m_width*3;
					 // 0 padding 때문에 더 크게 잡은걸까?
					 int height = h + (8*hFactor) - (h%(8*hFactor));
					 int width  = w + (8*vFactor) - (w%(8*vFactor));
					 jdata->m_rgb = new unsigned char[width * height];

					 memset(jdata->m_rgb, 0, width*height);
				 }

				 jdata->m_component_info[0].m_previousDC = 0;
				 jdata->m_component_info[1].m_previousDC = 0;
				 jdata->m_component_info[2].m_previousDC = 0;
				 jdata->m_component_info[3].m_previousDC = 0;

				 int xstride_by_mcu = 8*hFactor;
				 int ystride_by_mcu = 8*vFactor;

				 int jpgHeight = (int)jdata->m_height;
				 int jpgWidth = (int)jdata->m_width;

				 int cnt=0;
				 // Just the decode the image by 'macroblock' (size is 8x8, 8x16, or 16x16)
				 for (int y=0 ; y<jpgHeight; y+=ystride_by_mcu)
				 {
					 for (int x=0; x<jpgWidth; x+=xstride_by_mcu)
					 {
						 cnt++;
						 jdata->m_colourspace = jdata->m_rgb + x*3 + (y *jdata->m_width*3);

						 // Decode MCU Plane
						 DecodeMCU(jdata, hFactor, vFactor );

						 YCrCB_to_RGB24_Block8x8(jdata, hFactor, vFactor, x, y, jdata->m_width, jdata->m_height);
					 }
				 }
				 return true;
			 }
			 inline void YCrCB_to_RGB24_Block8x8(stJpegData *jdata, int w, int h, int imgx, int imgy, int imgw, int imgh)
			 {
				 const unsigned char *Y, *Cb, *Cr;
				 unsigned char *pix;

				 int r, g, b;

				 Y  = jdata->m_Y;
				 Cb = jdata->m_Cb;
				 Cr = jdata->m_Cr;

				 int olw = 0; // overlap
				 if ( imgx > (imgw-8*w) )
				 {
					 olw = imgw-imgx;
				 }

				 int olh = 0; // overlap
				 if ( imgy > (imgh-8*h) )
				 {
					 olh = imgh-imgy;
				 }

				 for (int y=0; y<(8*h - olh); y++)
				 {
					 for (int x=0; x<(8*w - olw); x++)
					 {
						 int poff = x*3 + jdata->m_width*3*y;
						 pix = &(jdata->m_colourspace[poff]);

						 int yoff = x + y*(w*8);
						 int coff = (int)(x*(1.0f/w)) + (int)(y*(1.0f/h))*8;

						 int yc =  Y[yoff];
						 int cb = Cb[coff];
						 int cr = Cr[coff];

						 ConvertYCrCbtoRGB(yc,cr,cb,&r,&g,&b);

						 pix[0] = Clamp(r);
						 pix[1] = Clamp(g);
						 pix[2] = Clamp(b);
					 }

				 }
			 }

			 inline void JpegGetImageSize(stJpegData *jdata, unsigned int *width, unsigned int *height)
			 {
				 *width  = jdata->m_width;
				 *height = jdata->m_height;
			 }
			 inline void WriteBMP24(const char* szBmpFileName, int Width, int Height, unsigned char* RGB)
			 {

				 // Round up the width to the nearest DWORD boundary
				 int iNumPaddedBytes = 4 - (Width * 3) % 4;
				 iNumPaddedBytes = iNumPaddedBytes % 4;

				 stBMFH bh;
				 memset(&bh, 0, sizeof(bh));
				 bh.bmtype[0]='B';
				 bh.bmtype[1]='M';
				 bh.iFileSize = (Width*Height*3) + (Height*iNumPaddedBytes) + sizeof(bh);
				 bh.iOffsetBits = sizeof(stBMFH);
				 bh.iSizeHeader = 40;
				 bh.iPlanes = 1;
				 bh.iWidth = Width;
				 bh.iHeight = Height;
				 bh.iBitCount = 24;
				 bh.iSizeImage = (Width*Height*3) + (Height*iNumPaddedBytes);

				 FILE* fp = fopen(szBmpFileName, "wb");
				 fwrite(&bh, sizeof(bh), 1, fp);
				 for (int y=Height-1; y>=0; y--)
				 {
					 for (int x=0; x<Width; x++)
					 {
						 int i = (x + (Width)*y) * 3;
						 unsigned int rgbpix = (RGB[i]<<16)|(RGB[i+1]<<8)|(RGB[i+2]<<0);
						 fwrite(&rgbpix, 3, 1, fp);
					 }
					 if (iNumPaddedBytes>0)
					 {
						 unsigned char pad = 0;
						 fwrite(&pad, iNumPaddedBytes, 1, fp);
					 }
				 } 
				 fclose(fp);
				 Windows::Forms::MessageBox::Show("이미지 저장완료...");
			 }

			 // jpg 버튼
			 /////////////////////////////////// 블록마다 그리기 ////////////////////////////////
	private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {

				 // Displays an OpenFileDialog so the user can select a Cursor.
				 OpenFileDialog ^ openFileDialog1 = gcnew OpenFileDialog();
				 openFileDialog1->Filter = "JPEG Files|*.jpg";
				 openFileDialog1->Title = "Select a JPEG File";

				 // Show the Dialog.
				 // If the user clicked OK in the dialog and
				 // a .CUR file was selected, open it.
				 if (openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK)
				 {
					 String^ path = openFileDialog1->FileName;

					 pat = toStandardString(path);

					 // 다시 불러올 경우 초기화
					 g_reservoir = 0;
					 g_nbits_in_reservoir = 0;

					 DrawJPGPixelBlock();

				 }

			 }

			 void DrawJPGPixelBlock()
			 {
				 if(!JPGParserBlock())
				 {
					 Windows::Forms::MessageBox::Show("Cannot Load JPEG...\n");
				 }
			 }
			 inline bool JPGParserBlock()
			 {
				 FILE *fp;
				 unsigned int lengthOfFile;
				 unsigned char *buf;


				 // Load the Jpeg into memory
				 fp = fopen(pat.c_str(), "rb");

				 lengthOfFile = FileSize(fp);
				 buf = new unsigned char[lengthOfFile + 4];

				 fread(buf, lengthOfFile, 1, fp);
				 fclose(fp);

				 if(DecodeJpgFileData(buf, lengthOfFile))
				 {
					 return true;
				 }

				 return false;

			 }

			 bool DecodeJpgFileData(const unsigned char* buf, const int sizeBuf)
			 {
				 stJpegData* jdec = new stJpegData();

				 if (!JpegParseHeader(jdec, buf, sizeBuf))
				 {
					 Windows::Forms::MessageBox::Show("ERROR > parsing jpg header\n");
					 return false;
				 }

				 JpegDecodeBlock(jdec);

				 return true;

			 }
			 inline bool JpegDecodeBlock(stJpegData *jdata)
			 {
				 int hFactor = jdata->m_component_info[cY].m_hFactor;
				 int vFactor = jdata->m_component_info[cY].m_vFactor;

				 // RGB24:
				 if (jdata->m_rgb == NULL)
				 {
					 int h = jdata->m_height*3;
					 int w = jdata->m_width*3;
					 // 0 padding 때문에 더 크게 잡은걸까?
					 int height = h + (8*hFactor) - (h%(8*hFactor));
					 int width  = w + (8*vFactor) - (w%(8*vFactor));
					 jdata->m_rgb = new unsigned char[width * height];

					 memset(jdata->m_rgb, 0, width*height);
				 }

				 jdata->m_component_info[0].m_previousDC = 0;
				 jdata->m_component_info[1].m_previousDC = 0;
				 jdata->m_component_info[2].m_previousDC = 0;
				 jdata->m_component_info[3].m_previousDC = 0;

				 int xstride_by_mcu = 8*hFactor;
				 int ystride_by_mcu = 8*vFactor;

				 int jpgHeight = (int)jdata->m_height;
				 int jpgWidth = (int)jdata->m_width;


				 Bitmap ^ bmp = gcnew Bitmap(jpgWidth, jpgHeight);
				 pictureBox1->Width::set(jpgWidth);
				 pictureBox1->Height::set(jpgHeight);

				 int cnt=0;
				 // Just the decode the image by 'macroblock' (size is 8x8, 8x16, or 16x16)
				 for (int y=0 ; y<jpgHeight; y+=ystride_by_mcu)
				 {
					 for (int x=0; x<jpgWidth; x+=xstride_by_mcu)
					 {
						 cnt++;
						 //jdata->m_colourspace = jdata->m_rgb + x*3 + (y *jdata->m_width*3);

						 // Decode MCU Plane
						 DecodeMCU(jdata, hFactor, vFactor );

						 YCrCB_to_RGB24_Block8x82(jdata, hFactor, vFactor, x, y, jdata->m_width, jdata->m_height, bmp);
					 }
				 }
				 return true;
			 }
			 inline void YCrCB_to_RGB24_Block8x82(stJpegData *jdata, int w, int h, int imgx, int imgy, int imgw, int imgh, Bitmap^ bitmap)
			 {
				 const unsigned char *Y, *Cb, *Cr;
				 // 최대 16x16블록...256 * RGB = 768byte
				 unsigned char blockData[768];



				 int r, g, b, cnt;

				 cnt = 0;

				 Y  = jdata->m_Y;
				 Cb = jdata->m_Cb;
				 Cr = jdata->m_Cr;

				 int olw = 0; // overlap
				 if ( imgx > (imgw-8*w) )
				 {
					 olw = imgw-imgx;
				 }

				 int olh = 0; // overlap
				 if ( imgy > (imgh-8*h) )
				 {
					 olh = imgh-imgy;
				 }

				 for (int y=0; y<(8*h - olh); y++)
				 {
					 for (int x=0; x<(8*w - olw); x++)
					 {
						 int yoff = x + y*(w*8);
						 int coff = (int)(x*(1.0f/w)) + (int)(y*(1.0f/h))*8;

						 int yc =  Y[yoff];
						 int cb = Cb[coff];
						 int cr = Cr[coff];

						 ConvertYCrCbtoRGB(yc,cr,cb,&r,&g,&b);

						 blockData[cnt] = Clamp(r);
						 blockData[cnt+1] = Clamp(g);
						 blockData[cnt+2] = Clamp(b);
						 cnt += 3;

					 }

				 }


				 DrawBlockData(bitmap, blockData, (8*w - olw) , (8*w - olh), imgx, imgy); 
			 }
			 void DrawBlockData(Bitmap ^bmp, unsigned char *buffer, int mcu_x, int mcu_y, int x, int y)
			 {
				 int cnt = 0;
				 for(int j=0;j<mcu_y;j++)
				 {
					 for(int i=0;i<mcu_x;i++)
					 {
						 // 버퍼의 Red값, Green값, Blue값으로 색을 만들자
						 Color rColor = Color::FromArgb(buffer[cnt], buffer[cnt+1], buffer[cnt+2]);

						 // 픽셀그림
						 // 8x8 블록이 이미지크기를 넘어갈 경우 그리지 않는다
						 if(i+x<bmp->Width && j+y<bmp->Height)
							 bmp->SetPixel(i + x, j + y, rColor);

						 cnt += 3;
					 }
				 }

				 //
				 pictureBox1->Image = bmp;
				 pictureBox1->Refresh();
			 }


			 /*********/
			 /* 공통 */
			 /********/
			 // path 받아옴
			 static std::string toStandardString(System::String^ string)
			 {
				 using System::Runtime::InteropServices::Marshal;
				 System::IntPtr pointer = Marshal::StringToHGlobalAnsi(string);
				 char* charPointer = reinterpret_cast<char*>(pointer.ToPointer());
				 std::string returnString(charPointer, string->Length);
				 Marshal::FreeHGlobal(pointer);
				 return returnString;
			 }
			 inline int FileSize(FILE *fp)
			 {
				 long pos;
				 fseek(fp, 0, SEEK_END);
				 pos = ftell(fp);
				 fseek(fp, 0, SEEK_SET);
				 return pos;
			 }
			 int DetermineSign(int val, int nBits)
			 {
				 bool negative = val < (1<<(nBits-1));

				 if (negative)
				 {
					 val = val + (-1 << (nBits)) + 1; 
				 }
				 return val;
			 }


			 // header parsing
			 inline bool JpegParseHeader(stJpegData *jdata, const unsigned char *buf, unsigned int size)
			 {
				 if ((buf[0] != 0xFF) || (buf[1] != SOI))
				 {
					 Windows::Forms::MessageBox::Show("Not a JPG file ?\n");
					 return false;
				 }
				 const unsigned char* startStream = buf+2;
				 const int fileSize = size-2;

				 return ParseJFIF(jdata, startStream);
			 }
			 // 본격파싱 시작
			 inline bool ParseJFIF(stJpegData *jdata, const unsigned char *stream)
			 {
				 int chuck_len;
				 int marker;
				 int sos_marker_found = 0;
				 int dht_marker_found = 0;

				 // Parse marker
				 while (!sos_marker_found)
				 {
					 if (*stream++ != 0xff)
					 {
						 int a = 0;
						 goto bogus_jpeg_format;
					 }
					 while (*stream == 0xff)
					 {
						 stream++;
					 }

					 marker = *stream++;
					 chuck_len = BYTE_TO_WORD(stream);

					 switch (marker)
					 {
					 case SOF:
						 {
							 ParseSOF(jdata, stream);
						 }
						 break;

					 case DQT:
						 {
							 if (!ParseDQT(jdata, stream))
								 return false;
						 }
						 break;

					 case SOS:
						 {
							 if (!ParseSOS(jdata, stream))
								 return false;
							 sos_marker_found = 1;
						 }
						 break;

					 case DHT:
						 {
							 if (!ParseDHT(jdata, stream))
								 return false;
							 dht_marker_found = 1;
						 }
						 break;

						 // The reason I added these additional skips here, is because for
						 // certain jpg compressions, like swf, it splits the encoding 
						 // and image data with SOI & EOI extra tags, so we need to skip
						 // over them here and decode the whole image
					 case SOI:
					 case EOI:
						 {
							 chuck_len = 0;
							 break;
						 }
						 break;

					 case 0xDD: //DRI: Restart_markers=1;
						 {
							 //dprintf("DRI - Restart_marker\n");
						 }
						 break;

					 case APP0:
						 {
							 //dprintf("APP0 Chunk ('txt' information) skipping\n");
						 }
						 break;

					 default:
						 {
							 //dprintf("ERROR> Unknown marker %2.2x\n", marker);
						 }
						 break;
					 }

					 stream += chuck_len;
				 }

				 if (!dht_marker_found) 
				 {
					 Windows::Forms::MessageBox::Show("ERROR> No Huffman table loaded\n");
					 return false;
				 }

				 return true;

bogus_jpeg_format:
				 Windows::Forms::MessageBox::Show("ERROR> Bogus jpeg format\n");
				 return false;

				 //return true;
			 }


			 inline void ParseSOF(stJpegData *jdata, const unsigned char *stream)
			 {
				 /*
				 SOF		16		0xffc0		Start Of Frame
				 Lf		16		3Nf+8		Frame header length
				 P		8		8			Sample precision
				 Y		16		0-65535		Number of lines
				 X		16		1-65535		Samples per line
				 Nf		8		1-255		Number of image components (e.g. Y, U and V).

				 ---------Repeats for the number of components (e.g. Nf)-----------------
				 Ci		8		0-255		Component identifier
				 Hi		4		1-4			Horizontal Sampling Factor
				 Vi		4		1-4			Vertical Sampling Factor
				 Tqi		8		0-3			Quantization Table Selector.
				 */

				 int height = BYTE_TO_WORD(stream+3);
				 int width  = BYTE_TO_WORD(stream+5);
				 int nr_components = stream[7];

				 stream += 8;
				 for (int i=0; i<nr_components; i++) 
				 {
					 int cid				= *stream++;
					 int sampling_factor = *stream++;
					 int Q_table			= *stream++;

					 stComponent *c = &jdata->m_component_info[cid];
					 c->m_vFactor = sampling_factor&0xf;
					 c->m_hFactor = sampling_factor>>4;
					 c->m_qTable = jdata->m_Q_tables[Q_table];

				 }
				 jdata->m_width = width;
				 jdata->m_height = height;
			 }
			 inline bool ParseDQT(stJpegData *jdata, const unsigned char *stream)
			 {
				 int length, qi;
				 float *table;

				 length = BYTE_TO_WORD(stream) - 2;
				 stream += 2;	// Skip length

				 while (length>0)
				 {
					 qi = *stream++;

					 int qprecision = qi>>4;	 // upper 4 bits specify the precision
					 int qindex     = qi&0xf; // index is lower 4 bits

					 if (qprecision)
					 {
						 // precision in this case is either 0 or 1 and indicates the precision 
						 // of the quantized values;
						 // 8-bit (baseline) for 0 and  up to 16-bit for 1 

						 Windows::Forms::MessageBox::Show("Error - 16 bits quantization table is not supported\n");
						 return false;
					 }

					 if (qindex>4)
					 {
						 System::String^ strText("Error - No more 4 quantization table is supported (got " + qi + ")\n");
						 Windows::Forms::MessageBox::Show(strText);
						 return false;
					 }

					 // The quantization table is the next 64 bytes
					 table = jdata->m_Q_tables[qindex];

					 // the quantization tables are stored in zigzag format, so we
					 // use this functino to read them all in and de-zig zag them
					 BuildQuantizationTable(table, stream);
					 stream += 64;
					 length -= 65;
				 }
				 return true;
			 }


			 inline void BuildQuantizationTable(float *qtable, const unsigned char *ref_table)
			 {
				 int c = 0;

				 for (int i=0; i<8; i++) 
				 {
					 for (int j=0; j<8; j++) 
					 {
						 unsigned char val = ref_table[c];

						 qtable[c] = val;
						 c++;
					 }
				 }
			 }
			 inline bool ParseSOS(stJpegData *jdata, const unsigned char *stream)
			 {
				 /*
				 SOS		16		0xffd8			Start Of Scan
				 Ls		16		2Ns + 6			Scan header length
				 Ns		8		1-4				Number of image components
				 Csj		8		0-255			Scan Component Selector
				 Tdj		4		0-1				DC Coding Table Selector
				 Taj		4		0-1				AC Coding Table Selector
				 Ss		8		0				Start of spectral selection
				 Se		8		63				End of spectral selection
				 Ah		4		0				Successive Approximation Bit High
				 Ai		4		0				Successive Approximation Bit Low
				 */

				 unsigned int nr_components = stream[2];


				 if (nr_components != 3)
				 {
					 Windows::Forms::MessageBox::Show("Error - We only support YCbCr image\n");
					 return false;
				 }


				 stream += 3;
				 for (unsigned int i=0;i<nr_components;i++) 
				 {
					 unsigned int cid   = *stream++;
					 unsigned int table = *stream++;

					 if ((table&0xf)>=4)
					 {
						 Windows::Forms::MessageBox::Show("Error - We do not support more than 2 AC Huffman table\n");
						 return false;
					 }
					 if ((table>>4)>=4)
					 {
						 Windows::Forms::MessageBox::Show("Error - We do not support more than 2 DC Huffman table\n");
						 return false;
					 }

					 jdata->m_component_info[cid].m_acTable = &jdata->m_HTAC[table&0xf];
					 jdata->m_component_info[cid].m_dcTable = &jdata->m_HTDC[table>>4];
				 }
				 jdata->m_stream = stream+3;
				 return true;
			 }

			 inline bool ParseDHT(stJpegData *jdata, const unsigned char *stream)
			 {
				 /*
				 u8 0xff 
				 u8 0xc4 (type of segment) 
				 u16 be length of segment 
				 4-bits class (0 is DC, 1 is AC, more on this later) 
				 4-bits table id 
				 array of 16 u8 number of elements for each of 16 depths 
				 array of u8 elements, in order of depth 
				 */

				 unsigned int count, i;
				 unsigned char huff_bits[17];
				 int length, index;

				 length = BYTE_TO_WORD(stream) - 2;
				 stream += 2;	// Skip length

				 while (length>0) 
				 {
					 index = *stream++;

					 // We need to calculate the number of bytes 'vals' will takes
					 huff_bits[0] = 0;
					 count = 0;
					 for (i=1; i<17; i++) 
					 {
						 huff_bits[i] = *stream++;
						 count += huff_bits[i];
					 }

					 if (count > 256)
					 {
						 Windows::Forms::MessageBox::Show("Error - No more than 1024 bytes is allowed to describe a huffman table");
						 return false;
					 }
					 if ( (index &0xf) >= HUFFMAN_TABLES)
					 {
						 System::String^ strText("Error - No mode than " + HUFFMAN_TABLES + " Huffman tables is supported"); 
						 Windows::Forms::MessageBox::Show(strText);
						 return false;
					 }

					 if (index & 0xf0 )
					 {
						 unsigned char* huffval = jdata->m_HTAC[index&0xf].m_hufVal;
						 for (i = 0; i < count; i++)
							 huffval[i] = *stream++;

						 BuildHuffmanTable(huff_bits, stream, &jdata->m_HTAC[index&0xf]); // AC
					 }
					 else
					 {
						 unsigned char* huffval = jdata->m_HTDC[index&0xf].m_hufVal;
						 for (i = 0; i < count; i++)
							 huffval[i] = *stream++;

						 BuildHuffmanTable2(huff_bits, stream, &jdata->m_HTDC[index&0xf]); // DC
					 }

					 length -= 1;
					 length -= 16;
					 length -= count;
				 }

				 return true;
			 }








			 inline void BuildHuffmanTable(const unsigned char *bits, const unsigned char *stream, stHuffmanTable *HT)
			 {
				 for (int j=1; j<=16; j++) 
				 {
					 HT->m_length[j] = bits[j];
				 }

				 // Work out the total number of codes
				 int numBlocks = 0;
				 for (int i=1; i<=16; i++)
				 {
					 numBlocks += HT->m_length[i];
				 }
				 HT->m_numBlocks = numBlocks;

				 // Fill in the data our our blocks, so we know how many bits each
				 // one is
				 int c=0;
				 for (int i=1; i<=16; i++)
				 {
					 for (int j=0; j<HT->m_length[i]; j++)
					 {
						 HT->m_blocks[c].length = i;
						 c++;
					 }

				 }

				 GenHuffCodes(HT->m_numBlocks, HT->m_blocks, HT->m_hufVal);
			 }

			 inline void BuildHuffmanTable2(const unsigned char *bits, const unsigned char *stream, stHuffmanTable2 *HT)
			 {
				 for (int j=1; j<=16; j++) 
				 {
					 HT->m_length[j] = bits[j];
				 }

				 // Work out the total number of codes
				 int numBlocks = 0;
				 for (int i=1; i<=16; i++)
				 {
					 numBlocks += HT->m_length[i];
				 }
				 HT->m_numBlocks = numBlocks;

				 // Fill in the data our our blocks, so we know how many bits each
				 // one is
				 int c=0;
				 for (int i=1; i<=16; i++)
				 {
					 for (int j=0; j<HT->m_length[i]; j++)
					 {
						 HT->m_blocks[c].length = i;
						 c++;
					 }

				 }

				 GenHuffCodes(HT->m_numBlocks, HT->m_blocks, HT->m_hufVal);
			 }

			 void GenHuffCodes( int num_codes, stBlock* arr, unsigned char* huffVal )
			 {
				 int hufcounter = 0;
				 int codelengthcounter = 1;


				 for(int cc=0; cc< num_codes; cc++)
				 {
					 while ( arr[cc].length > codelengthcounter )
					 {
						 hufcounter = hufcounter << 1;
						 codelengthcounter++;
					 }

					 arr[cc].code = hufcounter;
					 arr[cc].value = huffVal[cc];
					 hufcounter = hufcounter + 1;
				 }
			 }


			 /********************/
			 /*    JPEG Decode   */
			 /********************/


			 /***************************************************************************/
			 //
			 //  Decoding
			 //  .-------.
			 //  | 1 | 2 |
			 //  |---+---|
			 //  | 3 | 4 |
			 //  `-------'
			 //
			 /***************************************************************************/
			 // w : hFactor
			 // h : vFactor
			 // n * n MCU를 1차원배열에 순서대로 저장하기 위해
			 // ㅁㅁ   ->  1, 2 
			 // ㅁㅁ        3, 4 순서
			 inline void DecodeMCU(stJpegData *jdata, int w, int h)
			 {
				 // Y
				 for (int y=0; y<h; y++)
				 {
					 for (int x=0; x<w; x++)
					 {
						 int stride = w*8;
						 int offset = x*8 + y*64*w;

						 ProcessHuffmanDataUnit(jdata, cY);
						 DecodeSingleBlock(&jdata->m_component_info[cY], &jdata->m_Y[offset], stride);

					 }
				 }

				 // Cb
				 ProcessHuffmanDataUnit(jdata, cCb);
				 DecodeSingleBlock(&jdata->m_component_info[cCb], jdata->m_Cb, 8);
				 // Cr
				 ProcessHuffmanDataUnit(jdata, cCr);
				 DecodeSingleBlock(&jdata->m_component_info[cCr], jdata->m_Cr, 8);
			 }

			 /***************************************************************************/
			 void ProcessHuffmanDataUnit(stJpegData *jdata, int indx)
			 {
				 stComponent *c = &jdata->m_component_info[indx];

				 // Start Huffman decoding

				 // We memset it here, as later on we can just skip along, when we have lots
				 // of leading zeros, for our AC run length encoding :)
				 short DCT_tcoeff[64];
				 memset(DCT_tcoeff, 0, sizeof(DCT_tcoeff)); //Initialize DCT_tcoeff


				 bool found = false;
				 int decodedValue = 0;

				 // First thing is get the 1 DC coefficient at the start of our 64 element
				 // block
				 for (int k=1; k<16; k++)
				 {

					 // Keep grabbing one bit at a time till we find one thats a huffman code
					 int code = LookNBits(&jdata->m_stream, k);

					 // Check if its one of our huffman codes
					 if (IsInHuffmanCodes(code, k,  c->m_dcTable->m_numBlocks, c->m_dcTable->m_blocks, &decodedValue))
					 {
						 // Skip over the rest of the bits now.
						 SkipNBits(&jdata->m_stream, k);

						 found = true;

						 // The decoded value is the number of bits we have to read in next
						 int numDataBits = decodedValue;

						 // We know the next k bits are for the actual data
						 if (numDataBits==0)
						 {
							 DCT_tcoeff[0] = c->m_previousDC;
						 }
						 else
						 {

							 short data = GetNBits(&jdata->m_stream, numDataBits);

							 // 비트값을 2의보수로 바꾼 후 10진수로 변환 
							 data = DetermineSign(data, numDataBits);

							 DCT_tcoeff[0] = data + c->m_previousDC;
							 c->m_previousDC = DCT_tcoeff[0];
						 }

						 // Found so we can exit out
						 break;
					 }
				 }

				 if (!found)
				 {
					 Windows::Forms::MessageBox::Show("-|- ##ERROR## We have a *serious* error, unable to find huffman code\n");
				 }

				 // Second, the 63 AC coefficient
				 int nr=1; 
				 bool EOB_found=false;
				 while ( (nr<=63)&&(!EOB_found) )
				 {
					 int k = 0;
					 for (k=1; k<=16; k++)
					 {
						 // Keep grabbing one bit at a time till we find one thats a huffman code
						 int code = LookNBits(&jdata->m_stream, k);


						 // Check if its one of our huffman codes
						 if (IsInHuffmanCodes(code, k,  c->m_acTable->m_numBlocks, c->m_acTable->m_blocks, &decodedValue))
						 {

							 // Skip over k bits, since we found the huffman value
							 SkipNBits(&jdata->m_stream, k);


							 // Our decoded value is broken down into 2 parts, repeating RLE, and then
							 // the number of bits that make up the actual value next
							 int valCode = decodedValue;

							 unsigned char size_val = valCode&0xF;	// Number of bits for our data
							 unsigned char count_0  = valCode>>4;	// Number RunLengthZeros

							 if (size_val==0) 
							 {// RLE 
								 if (count_0==0)EOB_found=true;	// EOB found, go out
								 else if (count_0==0xF) nr+=16;  // skip 16 zeros
							 }
							 else
							 {
								 nr+=count_0; //skip count_0 zeroes

								 if (nr > 63)
								 {
									 Windows::Forms::MessageBox::Show("-|- ##ERROR## Huffman Decoding\n");
								 }

								 short data = GetNBits(&jdata->m_stream, size_val );

								 data = DetermineSign(data, size_val);

								 DCT_tcoeff[nr++]=data;

							 }
							 break;
						 }
					 }

					 if (k>16)
					 {	
						 nr++;
					 }
				 }

				 // We've decoded a block of data, so copy it across to our buffer
				 for (int j = 0; j < 64; j++)
				 {
					 c->m_DCT[j] = DCT_tcoeff[j];
				 }

			 }
			 bool IsInHuffmanCodes(int code, int numCodeBits, int numBlocks, stBlock* blocks, int* outValue)
			 {
				 // 허프만 테이블 0번 인덱스부터 검색
				 for (int j=0; j<numBlocks; j++)
				 {
					 // 허프만테이블 값
					 int hufhCode		= blocks[j].code; // code(2진수)
					 int hufCodeLenBits	= blocks[j].length; // code의 length 
					 int hufValue		= blocks[j].value; // symbol

					 // code와 code length가 같으면 symbol을 포인터로 잡아놓고 나중에 저장
					 if ((code==hufhCode) && (numCodeBits==hufCodeLenBits))
					 {
						 *outValue = hufValue;
						 return true;
					 }
				 }
				 return false;
			 }


			 inline void DecodeSingleBlock(stComponent *comp, unsigned char *outputBuf, int stride)
			 {
				 short* inptr    = comp->m_DCT;
				 float* quantptr = comp->m_qTable;


				 // Create a temp 8x8, i.e. 64 array for the data
				 int data[64] = {0};

				 // Copy our data into the temp array
				 for (int i=0; i<64; i++)
				 {
					 data[i] = inptr[i];
				 }

				 // De-Quantize
				 DequantizeBlock(data, quantptr);

				 // De-Zig-Zag
				 int block[64] = {0};
				 DeZigZag(block, data);

				 // Create an 8x8 array
				 int arrayBlock[8][8]={0};
				 TransformArray(arrayBlock, block);

				 // Inverse DCT
				 int val[8][8]={0};
				 PerformIDCT(val, arrayBlock);

				 // Level Shift each element (i.e. add 128), and copy to our
				 // output
				 unsigned char *outptr = outputBuf;
				 for (int y = 0; y < 8; y++) 
				 {
					 for (int x=0; x<8; x++)
					 {
						 val[x][y] += 128;

						 outptr[x] = Clamp(val[x][y]);
					 }

					 outptr += stride;
				 }
			 }

			 /********************/
			 /*    else Decode   */
			 /********************/
			 // Clamp our integer between 0 and 255
			 inline unsigned char Clamp(int i)
			 {
				 if (i<0)
					 return 0;
				 else if (i>255)
					 return 255;
				 else
					 return i;
			 } 
			 inline void FillNBits(const unsigned char** stream, int& nbits_wanted)
			 {
				 while ((int)g_nbits_in_reservoir<nbits_wanted)
				 {
					 const unsigned char c = *(*stream)++;
					 g_reservoir <<= 8;
					 if (c == 0xff && (**stream) == 0x00)
						 (*stream)++;
					 g_reservoir |= c;
					 g_nbits_in_reservoir+=8;
				 }
			 }

			 inline short GetNBits(const unsigned char** stream, int nbits_wanted)
			 {
				 FillNBits(stream, nbits_wanted);

				 short result = ((g_reservoir)>>(g_nbits_in_reservoir-(nbits_wanted))); 

				 g_nbits_in_reservoir -= (nbits_wanted); 
				 g_reservoir &= ((1U<<g_nbits_in_reservoir)-1);

				 /*
				 // Could do the sign conversion here!
				 if (result < (short)(1UL<<((nbits_wanted)-1)))
				 {
				 result = result + (short)(0xFFFFFFFFUL<<(nbits_wanted))+1;
				 }
				 */
				 return result;
			 }

			 inline int LookNBits(const unsigned char** stream, int nbits_wanted)
			 {
				 FillNBits(stream, nbits_wanted);

				 int result = ((g_reservoir)>>(g_nbits_in_reservoir-(nbits_wanted)));
				 return result;
			 }

			 inline void SkipNBits(const unsigned char** stream, int& nbits_wanted)
			 {
				 FillNBits(stream, nbits_wanted);

				 g_nbits_in_reservoir -= (nbits_wanted); 
				 g_reservoir &= ((1U<<g_nbits_in_reservoir)-1);
			 }

			 /********************/
			 /*    Decode Step   */
			 /********************/
			 float C(int u)
			 {
				 if (u == 0)
					 return (1.0f/sqrtf(2));
				 else
					 return 1.0f;
			 }
			 int func(int x, int y, const int block[8][8])
			 {
				 const float PI = 3.14f;
				 float sum=0;
				 for( int u=0; u<8; u++)
				 {
					 for(int v=0; v<8; v++)
					 {
						 //sum += ( C(u) * C(v) ) * block[u][v] * cosf( ((2*x+1) * u * PI) / 16)  * cosf( ((2*y+1) * v * PI) / 16);
						 sum += ( C(u) * C(v) ) * block[u][v] * cos1[x][u]  * cos1[y][v];
					 }
				 }         
				 //return (int) ((1.0/4.0) * sum);
				 return (int)sum >> 2;
			 }

			 void PerformIDCT(int outBlock[8][8], const int inBlock[8][8])
			 {
				 for(int y=0; y<8; y++)
				 {
					 for(int x=0; x<8; x++)
					 {
						 outBlock[x][y]  =  func( x, y, inBlock);
					 }
				 }
			 }

			 void DequantizeBlock( int block[64], const float quantBlock[64] )
			 {
				 for( int c=0; c<64; c++)
				 {
					 block[c] = (int)(block[c] * quantBlock[c]);
				 }
			 }

			 void DeZigZag(int outBlock[64], const int inBlock[64])
			 {
				 for(int i=0; i<64; i++)
				 {
					 outBlock[ i ] = inBlock[ZigZagArray[i]];
				 }
			 }

			 void TransformArray(int outArray[8][8], const int inArray[64])
			 {
				 int cc = 0;
				 for( int y=0; y<8; y++)
				 {
					 for( int x=0; x<8; x++)
					 {
						 outArray[x][y]  =  inArray[cc];
						 cc++;
					 }
				 }
			 }


			 /**********************/
			 /*    YCbCr to RGB    */
			 /**********************/

			 inline void ConvertYCrCbtoRGB(int y, int cb, int cr, int* r, int* g, int* b)
			 {
				 float red, green, blue;

				 red   = y + 1.402f*(cb-128);
				 green = y-0.34414f*(cr-128)-0.71414f*(cb-128);
				 blue  = y+1.772f*(cr-128);

				 *r = (int) Clamp((int)red);
				 *g = (int) Clamp((int)green);
				 *b = (int) Clamp((int)blue);
			 }


#pragma PNG

	private: System::Void pngBtn_Click(System::Object^  sender, System::EventArgs^  e) {
				 // Displays an OpenFileDialog so the user can select a Cursor.
				 OpenFileDialog ^ openFileDialog1 = gcnew OpenFileDialog();
				 openFileDialog1->Filter = "PNG Files|*.png";
				 openFileDialog1->Title = "Select a png File";

				 // Show the Dialog.
				 // If the user clicked OK in the dialog and
				 // a .CUR file was selected, open it.
				 if (openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK)
				 {
					 String^ path = openFileDialog1->FileName;

					 pat = toStandardString(path);

					 DrawPngPixel();

				 }
			 }

			 void DrawPngPixel()
			 {
				 FILE *fp;
				 unsigned int lengthOfFile;
				 unsigned char *buf;


				 // Load the Jpeg into memory
				 fp = fopen(pat.c_str(), "rb");

				 lengthOfFile = FileSize(fp);
				 buf = new unsigned char[lengthOfFile + 4];

				 fread(buf, lengthOfFile, 1, fp);
				 fclose(fp);

				 DecodePngFileData(buf, lengthOfFile);

				 delete[] buf;
			 }
			 void DecodePngFileData(const unsigned char* buf, const int sizeBuf)
			 {
				 if(PngParseHeader(buf, sizeBuf)<0)
				 {
					 printf("ERROR > parsing png header\n");
					 exit(1);
				 }

			 }
			 int PngParseHeader(const unsigned char* buf, unsigned int size)
			 {
				 const unsigned char* startStream; // 스트림

				 // png identifier
				 if ((buf[0] != 0x89) || (buf[1] != 0x50) || (buf[2] != 0x4E) || (buf[3] != 0x47) || (buf[4] != 0x0D) || (buf[5] != 0x0A) || (buf[6] != 0x1A) || (buf[7] != 0x0A))
				 {
					 printf("Not a PNG file..\n");
					 return -1;
				 }
				 printf("Png File!! \n");


				 startStream = buf+8;
				 // 파싱 시작
				 if(ParsePng(buf, startStream) > 0)
				 {
					 printf("parse success!!\n");
					 return 0;
				 }
				 else
				 {
					 printf("parse fail!!\n");
					 return -1;
				 }
			 }
			 int ParsePng(const unsigned char* buffer, const unsigned char *stream)
			 {
				 int chunkLength;
				 int marker;
				 int end_marker = 0;

				 // png, bitmap 초기화
				 struct XPng *xpng;
				 struct XBitmap *xbitmap;

				 xpng = (struct XPng *)malloc(sizeof(struct XPng));
				 Init_png(xpng);

				 xbitmap = (struct XBitmap *)malloc(sizeof(struct XBitmap));
				 Init_Bmp(xbitmap);

				 xpng->m_pInflate = (struct XInflate *)calloc(1, sizeof(struct XInflate));
				 Init_inf(xpng->m_pInflate);

				 Bitmap^ bmp;

				 while(!end_marker)
				 {
					 chunkLength = BYTE_TO_INT(stream);
					 marker = BYTE_TO_INT(stream+4);

					 switch(marker)
					 {
					 case IHDR:
						 stream += 4 + 4; // data length(4) + chunk length(4)

						 printf("HEAD parsing..length : %d\n", chunkLength);
						 bmp = IHDRParse(xpng, xbitmap, stream);
						 stream += chunkLength; // chunk size
						 break;

					 case sRGB:
						 stream += 4 + 4; // data length(4) + chunk length(4)
						 printf("sRGB parsing..length : %d\n", chunkLength);
						 stream += chunkLength; // chunk size
						 break;

					 case pHYs:
						 stream += 4 + 4; // data length(4) + chunk length(4)
						 printf("pHYs parsing..length : %d\n", chunkLength);
						 stream += chunkLength; // chunk size
						 break;

					 case gAMA:
						 stream += 4 + 4; // data length(4) + chunk length(4)
						 printf("gAMA parsing..length : %d\n", chunkLength);
						 stream += chunkLength; // chunk size
						 break;

					 case PLTE:
						 stream += 4 + 4; // data length(4) + chunk length(4)
						 printf("PLTE parsing..length : %d\n", chunkLength);
						 PLTEParse(xpng, stream, chunkLength);
						 stream += chunkLength; // chunk size
						 break;

					 case IDAT:
						 stream += 4 + 4; // data length(4) + chunk length(4)
						 printf("IDAT parsing..length : %d\n", chunkLength);

						 if(!xpng->m_bEnd)
						 {
							 stream += 2;
							 chunkLength -= 2;

							 xpng->m_filter_b_dummy = (unsigned char*)calloc(1, xpng->m_imageStride);

							 // IDATParse 안에서 true로 바꾼다
							 //xpng->m_bEnd = true;
						 }

						 IDATParse(bmp, xpng, xpng->m_pInflate, xbitmap, stream, chunkLength);

						 stream += chunkLength; // chunk size
						 break;

					 case tIME:
						 stream += 4 + 4; // data length(4) + chunk length(4)
						 printf("tIME parsing..length : %d\n", chunkLength);
						 stream += chunkLength; // chunk size
						 break;

					 case tEXt:
						 stream += 4 + 4; // data length(4) + chunk length(4)
						 printf("tEXt parsing..length : %d\n", chunkLength);
						 stream += chunkLength; // chunk size
						 break;

					 case IEND:
						 end_marker = 1;

						 IENDParse(xbitmap);

						 printf("IEND.....\n");
						 break;
					 default:

						 break;
					 }

					 stream += 4; // CRC(4)


				 }

				 // png, bitmap 해제
				 Destroy_inf(xpng->m_pInflate);
				 Destory_png(xpng);
				 free(xpng);
				 free(xbitmap);


				 return 1;
			 }

			 Bitmap^ IHDRParse(struct XPng* xpng, struct XBitmap* xbitmap, const unsigned char* stream)
			 {
				 int pixelFormat;

				 xpng->m_header.width= BYTE_TO_INT(stream);
				 xpng->m_header.height = BYTE_TO_INT(stream+4);
				 xpng->m_header.bitDepth = BYTE_TO_CHAR(stream+8);
				 xpng->m_header.colorType = BYTE_TO_CHAR(stream+9);
				 xpng->m_header.compressionMethod = BYTE_TO_CHAR(stream+10);
				 xpng->m_header.filterMode = BYTE_TO_CHAR(stream+11);
				 xpng->m_header.interlaceMethod = BYTE_TO_CHAR(stream+12);

				 xpng->m_ColorMapTablePos = 0;
				 xpng->m_bPaletted = false;


				 if(xpng->m_header.colorType==COLORTYPE_COLOR_ALPHA)
				 {
					 xpng->m_ColorMapTableCur = colorMapTable_RGBA_2_BGRA;
					 xpng->m_ColorMapTableLen = sizeof(colorMapTable_RGBA_2_BGRA) / sizeof(colorMapTable_RGBA_2_BGRA[0]);	// 결국은 4
					 pixelFormat = PIXELFORMAT_32;
				 }
				 else if(xpng->m_header.colorType==COLORTYPE_COLOR)
				 {
					 xpng->m_ColorMapTableCur = colorMapTable_RGB_2_BGR;
					 xpng->m_ColorMapTableLen = sizeof(colorMapTable_RGB_2_BGR) / sizeof(colorMapTable_RGB_2_BGR[0]);		// 결국은 3
					 pixelFormat = PIXELFORMAT_24;
				 }
				 else if(xpng->m_header.colorType==COLORTYPE_COLOR_PAL)
				 {
					 // 빨레트를 쓴다. ALPHA 는 없다. 그래서 RGB 2 BGR 사용
					 xpng->m_ColorMapTableCur = colorMapTable_RGB_2_BGR;
					 xpng->m_ColorMapTableLen = sizeof(colorMapTable_RGB_2_BGR) / sizeof(colorMapTable_RGB_2_BGR[0]);		// 결국은 3
					 xpng->m_bPaletted = true;
					 pixelFormat = PIXELFORMAT_32;
				 }
				 else
				 {
				 }

				 CreateDib(xbitmap, xpng->m_header.width, xpng->m_header.height, pixelFormat);

				 SetDecodeData(xpng, xbitmap->m_pixel, xbitmap->m_stride, xbitmap->m_paddingByte, xbitmap->m_pixelFormat / 8);

				 pixX = 0;
				 pixY = 0;

				 pictureBox1->Width::set(xbitmap->m_width);
				 pictureBox1->Height::set(xbitmap->m_height);

				 return gcnew Bitmap(xbitmap->m_width, xbitmap->m_height);

			 }
			 void PLTEParse(struct XPng* png, const unsigned char* stream, int dataLength){

				 png->m_palette = (unsigned char*)malloc(dataLength);

				 memcpy(png->m_palette, stream, dataLength);
			 }
			 int IDATParse(Bitmap^ bmp, struct XPng* xpng, struct XInflate* inflate, struct XBitmap* xbitmap, const unsigned char* stream, int dataLength)
			 {

				 unsigned char* idatBuffer;

				 unsigned char*		imageCurBuffer;
				 int					imageCurWidthByte;
				 int					ColorMapTablePos;
				 unsigned char		filter;
				 unsigned char		filter_a[RGBA];
				 unsigned char*		filter_b ;	
				 unsigned char		filter_c[RGBA];
				 const unsigned char* outBuf;
				 unsigned int					outBufLen;
				 int					ColorMapTableLen;
				 int					imageWidthByte;
				 int					imagePaddingByte;
				 const int*			ColorMapTableCur;

				 imageCurBuffer = xpng->m_imageCurBuffer;
				 imageCurWidthByte = xpng->m_imageCurWidthByte;
				 ColorMapTablePos = xpng->m_ColorMapTablePos;
				 filter = xpng->m_filter;
				 filter_b = xpng->m_filter_b;	
				 *(int*)filter_a = *(int*)xpng->m_filter_a;
				 *(int*)filter_c = *(int*)xpng->m_filter_c;


				 ColorMapTableLen = xpng->m_ColorMapTableLen;
				 imageWidthByte = xpng->m_imageWidthByte;
				 imagePaddingByte = xpng->m_imagePaddingByte;
				 ColorMapTableCur = xpng->m_ColorMapTableCur;



				 if(xpng->m_pInflate->m_outBuffer == NULL)
				 {
					 xpng->m_pInflate->m_outBuffer = (unsigned char*)malloc(DEFAULT_ALLOC_SIZE); // jmlee 초기화
					 xpng->m_pInflate->m_outBufferAlloc = DEFAULT_ALLOC_SIZE;
				 }


				 idatBuffer = (unsigned char *)malloc(dataLength);

				 memcpy(idatBuffer, stream, dataLength);


				 if(Inflate(inflate, idatBuffer, dataLength)!=XINFLATE_ERR_OK){
					 printf("error!!!!");
				 }
				 //디코딩 결과 저장
				 outBuf = inflate->m_outBuffer;
				 outBufLen = inflate->m_outBufferPos;

				 // 출력 버퍼에 쓰기.
				 while(outBufLen)
				 {
					 // 매 스캔라인 첫번째 바이트는 filter 정보이다.
					 if(imageCurWidthByte == imageWidthByte)
					 {
						 filter = *outBuf;


						 // 필터에 사용할 변수 초기화
						 *((unsigned int*)filter_a) = 0;
						 *((unsigned int*)filter_c) = 0;
						 filter_b = xpng->m_scanline_current;

						 if(xpng->m_bEnd == false)
						 {
							 filter_b = xpng->m_filter_b_dummy;
							 xpng->m_bEnd = true;
						 }

						 // 기본 초기화
						 ColorMapTablePos = 0;

						 // 이전 스캔라인 기억 처리
						 xpng->m_scanline_before = xpng->m_scanline_current;
						 xpng->m_scanline_current = imageCurBuffer;

						 outBuf++;
						 outBufLen--;
					 }

					 // 영상 데이타 복사
					 // jmlee filter none만 처리함....
					 switch(filter)
					 {
					 case FILTER_NONE :

						 if(xpng->m_bPaletted)
						 {
							 unsigned char palPixel;
							 while(outBufLen)
							 {
								 palPixel = *outBuf;

								 RenderOnePixel(bmp, &xpng->m_palette[palPixel * 3]);

								 // 빨레트를 참고해서 RGBA 값 세팅하기
								 *(imageCurBuffer + 2) = xpng->m_palette[palPixel * 3 + 0];		// R
								 *(imageCurBuffer + 1) = xpng->m_palette[palPixel * 3 + 1];		// G
								 *(imageCurBuffer + 0) = xpng->m_palette[palPixel * 3 + 2];		// B

								 // RGBA 사용안함
								 *(imageCurBuffer + 3) = 205;					// A

								 MOVETONEXTBUFFER_4_PAL

							 }
						 }
						 else
						 {
							 if(ColorMapTablePos==0)
							 {
								 if(ColorMapTableCur == colorMapTable_RGB_2_BGR)
								 {
									 while(outBufLen>3 && imageCurWidthByte>3)
									 {

										 RenderOnePixel(bmp, outBuf);

										 *(imageCurBuffer + 2) = *(outBuf+0);		// R
										 *(imageCurBuffer + 1) = *(outBuf+1);		// G
										 *(imageCurBuffer + 0) = *(outBuf+2);		// B

										 MOVETONEXTBUFFER_FAST(3);
									 }
								 }
								 else if(ColorMapTableCur == colorMapTable_RGBA_2_BGRA)
								 {
									 while(outBufLen>4 && imageCurWidthByte>4)
									 {

										 RenderOnePixel(bmp, outBuf);

										 *(imageCurBuffer + 2) = *(outBuf+0);		// R
										 *(imageCurBuffer + 1) = *(outBuf+1);		// G
										 *(imageCurBuffer + 0) = *(outBuf+2);		// B
										 *(imageCurBuffer + 3) = *(outBuf+3);		// A

										 MOVETONEXTBUFFER_FAST(4);
									 }
								 }
							 }

							 while(outBufLen)
							 {
								 if((imageCurWidthByte % xbitmap->m_rgbSize) == 0)
									 RenderOnePixel(bmp, outBuf);

								 // RGB 를 BGR 로 혹은 RGBA 를 BGRA 로 바꾸기 위해서 테이블 참조
								 *(imageCurBuffer + ColorMapTableCur[ColorMapTablePos]) = *outBuf;

								 MOVETONEXTBUFFER
							 }
						 }

						 break;

					 case FILTER_SUB :

						 if(xpng->m_bPaletted){
						 }
						 else
						 {
							 // 한번에 3,4 바이트씩 복사한다.
							 if(ColorMapTablePos==0)
							 {
								 if(ColorMapTableCur == colorMapTable_RGB_2_BGR)
								 {
									 while(outBufLen>3 && imageCurWidthByte>3)
									 {
										 filter_a[0] = *(imageCurBuffer + 2) = *(outBuf+0) + filter_a[0];	// R
										 filter_a[1] = *(imageCurBuffer + 1) = *(outBuf+1) + filter_a[1];	// G
										 filter_a[2] = *(imageCurBuffer + 0) = *(outBuf+2) + filter_a[2];	// B

										 RenderOnePixelWithFilter(bmp, outBuf, filter_a);

										 MOVETONEXTBUFFER_FAST(3);
									 }
								 }
								 else if(ColorMapTableCur == colorMapTable_RGBA_2_BGRA)
								 {
									 while(outBufLen>4 && imageCurWidthByte>4)
									 {
										 

										 filter_a[0] = *(imageCurBuffer + 2) = *(outBuf+0) + filter_a[0];	// R
										 filter_a[1] = *(imageCurBuffer + 1) = *(outBuf+1) + filter_a[1];	// G
										 filter_a[2] = *(imageCurBuffer + 0) = *(outBuf+2) + filter_a[2];	// B
										 filter_a[3] = *(imageCurBuffer + 3) = *(outBuf+3) + filter_a[3];	// A

										 RenderOnePixelWithFilter(bmp, outBuf, filter_a);

										 MOVETONEXTBUFFER_FAST(4);
									 }
								 }
							 }

							 while(outBufLen)
							 {
								 if((imageCurWidthByte % xbitmap->m_rgbSize) == 0)
									 RenderOnePixelWithFilter(bmp, outBuf, filter_a);

								 // RGB 를 BGR 로 혹은 RGBA 를 BGRA 로 바꾸기 위해서 테이블 참조
								 // + 왼쪽 픽셀 참조
								 // + 필터값 저장(좀전 픽셀)
								 filter_a[ColorMapTablePos] = *(imageCurBuffer + ColorMapTableCur[ColorMapTablePos]) = *outBuf + filter_a[ColorMapTablePos];
								 MOVETONEXTBUFFER
							 }
						 }
						 break;

					 case FILTER_UP :

						 if(xpng->m_bPaletted){
						 }
						 else
						 {
							 if(filter_b==NULL)
							 {
								 //DOERR(XPNG_ERR_INVALID_DATA);		// 에러 상황이다.
							 }

							 // 한번에 3, 4바이트씩 복사한다.
							 if(ColorMapTablePos==0)
							 {
								 if(ColorMapTableCur == colorMapTable_RGB_2_BGR)
								 {
									 while(outBufLen>3 && imageCurWidthByte>3)
									 {
										 *(imageCurBuffer + 2) = *(outBuf+0) + *(filter_b + 2);	// R
										 *(imageCurBuffer + 1) = *(outBuf+1) + *(filter_b + 1);	// G
										 *(imageCurBuffer + 0) = *(outBuf+2) + *(filter_b + 0);	// B

										 RenderOnePixelWithFilter(bmp, outBuf, filter_b);

										 MOVETONEXTBUFFER_FAST(3);
										 filter_b+= 3;
									 }
								 }
								 else if(ColorMapTableCur == colorMapTable_RGBA_2_BGRA)
								 {
									 while(outBufLen>4 && imageCurWidthByte>4)
									 {
										 *(imageCurBuffer + 2) = *(outBuf+0) + *(filter_b + 2);	// R
										 *(imageCurBuffer + 1) = *(outBuf+1) + *(filter_b + 1);	// G
										 *(imageCurBuffer + 0) = *(outBuf+2) + *(filter_b + 0);	// B
										 *(imageCurBuffer + 3) = *(outBuf+3) + *(filter_b + 3);	// A

										 RenderOnePixelWithFilter(bmp, outBuf, filter_b);

										 MOVETONEXTBUFFER_FAST(4);
										 filter_b+= 4;
									 }
								 }
							 }

							 while(outBufLen)
							 {
								 if((imageCurWidthByte % xbitmap->m_rgbSize) == 0)
									 RenderOnePixelWithFilter(bmp, outBuf, filter_b);
								 // RGB 를 BGR 로 혹은 RGBA 를 BGRA 로 바꾸기 위해서 테이블 참조
								 // + 이전 라인 참조
								 *(imageCurBuffer + ColorMapTableCur[ColorMapTablePos]) = 
									 *outBuf +
									 *(filter_b + ColorMapTableCur[ColorMapTablePos]);					// 이전 스캔 라인

								 filter_b++;

								 MOVETONEXTBUFFER
							 }
						 }
						 break;


					 case FILTER_AVERAGE :

						 if(xpng->m_bPaletted){
						 }
						 else
						 {
							 if(filter_b==NULL)
							 {
								 //DOERR(XPNG_ERR_INVALID_DATA);		// 에러 상황이다.
							 }



							 // 한번에 3,4 픽셀 처리하기
							 if(ColorMapTablePos==0)
							 {
								 // todo
								 // 이거 테스트 해봐야 하는데.. 적당한 샘플이 없음.. average 는 별로 안쓰이는듯?
							 }

							 while(outBufLen)
							 {
								 int a, b;
								 unsigned char* dest;
								 // 참조할 데이타
								 a = filter_a[ColorMapTablePos];								// 왼쪽
								 b = *(filter_b + ColorMapTableCur[ColorMapTablePos]);		// 위

								 // 현재 출력 지점
								 dest = imageCurBuffer + ColorMapTableCur[ColorMapTablePos];

								 *dest = *outBuf + (a+b)/2;

								 // 좌측 업데이트
								 filter_a[ColorMapTablePos] = *dest;
								 // 상단 업데이트
								 filter_b++;

								 MOVETONEXTBUFFER
							 }
						 }
						 break;

					 case FILTER_PAETH :
						 if(xpng->m_bPaletted){
						 }
						 else
						 {

							 int a, b,c,pa,pb,pc;
							 int p;
							 unsigned char* dest;
							 int cur;

							 if(filter_b==NULL)
							 {
								 //DOERR(XPNG_ERR_INVALID_DATA);		// 에러 상황이다.
							 }
							 /////////////////////////////////////////////////////////////////////////////
							 //
							 // PAETH 처리를 매크로화
							 //
#define DO_PAETH(src, dst)													\
	a = filter_a[src]; b = *(filter_b +dst); c = filter_c[src];		\
	p = a + b - c;													\
	pa = XPNG_ABS(p-a);												\
	pb = XPNG_ABS(p-b);												\
	pc = XPNG_ABS(p-c);												\
	\
	/* 현재 출력 지점 */											\
	dest = imageCurBuffer + dst;									\
	\
	/* 디코딩된 값 */												\
	cur = *(outBuf+src);											\
	\
	/* 합치기 */													\
	if(pa<=pb && pa<=pc)	*dest = cur + a;						\
							 else if(pb<=pc)			*dest = cur + b;						\
							 else					*dest = cur + c;						\
							 \
							 /* 필터 업데이트 */												\
							 filter_a[src] = *dest;											\
							 filter_c[src] = b;


							 // 한번에 3,4 픽셀 처리하기
							 if(ColorMapTablePos==0)
							 {
								 if(ColorMapTableCur == colorMapTable_RGB_2_BGR)
								 {
									 while(outBufLen>3 && imageCurWidthByte>3)
									 {
										 DO_PAETH(0, 2);		// R
										 DO_PAETH(1, 1);		// G
										 DO_PAETH(2, 0);		// B

										 // 다음 픽셀로
										 MOVETONEXTBUFFER_FAST(3);
										 filter_b+= 3;
									 }
								 }
								 else if(ColorMapTableCur == colorMapTable_RGBA_2_BGRA)
								 {
									 while(outBufLen>4 && imageCurWidthByte>4)
									 {
										 DO_PAETH(0, 2);		// R
										 DO_PAETH(1, 1);		// G
										 DO_PAETH(2, 0);		// B
										 DO_PAETH(3, 3);		// A

										 // 다음 픽셀로
										 MOVETONEXTBUFFER_FAST(4);
										 filter_b+= 4;
									 }
								 }
							 }

							 while(outBufLen)
							 {
								 // 참조할 데이타
								 a = filter_a[ColorMapTablePos];								// 왼쪽
								 b = *(filter_b + ColorMapTableCur[ColorMapTablePos]);		// 위
								 c = filter_c[ColorMapTablePos];								// 왼쪽 위

								 p = a + b - c;

								 pa = XPNG_ABS(p-a);
								 pb = XPNG_ABS(p-b);
								 pc = XPNG_ABS(p-c);


								 // 현재 출력 지점
								 dest = imageCurBuffer + ColorMapTableCur[ColorMapTablePos];

								 // 디코딩된 값
								 cur = *outBuf;


								 // 합치기
								 if(pa<=pb && pa<=pc)
									 *dest = cur + a;
								 else if(pb<=pc)
									 *dest = cur + b;
								 else 
									 *dest = cur + c;

								 // 좌측 업데이트
								 filter_a[ColorMapTablePos] = *dest;
								 // 좌상단 업데이트
								 filter_c[ColorMapTablePos] = b;
								 // 상단 업데이트
								 filter_b++;

								 MOVETONEXTBUFFER
							 }
						 }
						 break;
					 default :
						 return false;
					 }
				 }

				 // 로컬 변수를 멤버 변수로 되돌리기
				 xpng->m_imageCurBuffer = imageCurBuffer;
				 xpng->m_imageCurWidthByte = imageCurWidthByte;
				 xpng->m_ColorMapTablePos = ColorMapTablePos ;

				 xpng->m_filter = filter;
				 xpng->m_filter_b = filter_b;
				 *(unsigned int*)xpng->m_filter_a = *(unsigned int*)filter_a;
				 *(unsigned int*)xpng->m_filter_c = *(unsigned int*)filter_c;


				 free((unsigned char *)idatBuffer);
				 // 한번에 바로 찍음
				 //WriteBMFile(xbitmap);
				 //WriteBMFile(xbitmap, xbitmap->m_pixel, 125182);


				 return true;

				 //++++++++++++++++++++++++++IDAT끝+++++++++++++++++++++++++++++++++++
				 //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++



			 }

			 void IENDParse(struct XBitmap* xbitmap)
			 {
				 //DrawPNGData(xbitmap->m_width, xbitmap->m_height, xbitmap->m_pixel , xbitmap->m_pixelFormat / 8);
			 }
			 // png 바로 그리기
			 void DrawPNGData(int width, int height, unsigned char* RGB, int isrgba)
			 {

				 int iNumPaddedBytes = 4 - (width * isrgba) % 4;
				 iNumPaddedBytes = iNumPaddedBytes % 4;

				 Bitmap ^ bmp = gcnew Bitmap(width, height);
				 pictureBox1->Width::set(width);
				 pictureBox1->Height::set(height);

				 unsigned char *rgbData = (unsigned char *)malloc(width*height*isrgba);

				 int offset = 0;
				 // 거꾸로 저장되어있는 Pixel 값을 역으로 저장
				 for (int y=height-1; y>=0; y--)
				 {
					 for (int x=0; x<width; x++)
					 {
						 int i = (x + (width)*y) * isrgba;
						 unsigned int rgbpix = (RGB[i]<<0)|(RGB[i+1]<<8)|(RGB[i+2]<<16);
						 memcpy(rgbData + offset, &rgbpix, isrgba);

						 offset += isrgba;
					 }
					 if (iNumPaddedBytes>0)
					 {
						 unsigned char pad = 0x00;
						 memcpy(rgbData + offset, &pad, iNumPaddedBytes);
						 offset += iNumPaddedBytes;

					 }
				 }

				 // 세로그리기
				 int arrayOffset = 0;
				 for(int i=0;i<height;i++)
				 {
					 // 버퍼의 옵셋 초기화(가로 한줄 다 읽으면 다음 가로 한줄을 읽기때문에 offset을 0으로 초기화)
					 int pixelOffset = 0;

					 unsigned char *widthData = (unsigned char *)malloc(width*isrgba);


					 memcpy(widthData, rgbData + arrayOffset, width*isrgba);

					 // 가로그리기
					 for(int j=0;j<width;j++)
					 {
						 RenderWidthPixel(bmp, widthData, pixelOffset, j, i, height);

						 pixelOffset += isrgba;
						 arrayOffset += isrgba;	

					 }

					 free(widthData);
				 }

				 pictureBox1->Image = bmp;


				 free(rgbData);

			 }
			 void RenderOnePixelWithFilter(Bitmap^ bmp, const unsigned char* buffer, const unsigned char* buffer2)
			 {
				 Color rColor = Color::FromArgb(buffer[0]+buffer2[0] >= 255 ? 255 : buffer[0]+buffer2[0], buffer[1]+buffer2[1] >= 255 ? 255 : buffer[1]+buffer2[1], buffer[2]+buffer2[2] >= 255 ? 255 : buffer[2]+buffer2[2]);
				 // 픽셀그림
				 bmp->SetPixel(pixX, pixY, rColor);

				 pixX++;


				 if(pixX >= bmp->Width)
				 {
					 pixX = 0;
					 pixY++;

				 }

				 pictureBox1->Image = bmp;
				 pictureBox1->Refresh();

			 }
			 void RenderOnePixel(Bitmap^ bmp, const unsigned char* buffer)
			 {


				 Color rColor = Color::FromArgb(buffer[0], buffer[1], buffer[2]);
				 // 픽셀그림
				 bmp->SetPixel(pixX, pixY, rColor);

				 pixX++;


				 if(pixX >= bmp->Width)
				 {
					 pixX = 0;
					 pixY++;
					 pictureBox1->Refresh();

				 }

				 pictureBox1->Image = bmp;


			 }
			 // 픽셀 그리기
	private: System::Void drawText_Click(System::Object^  sender, System::EventArgs^  e) 
			 {
				 Bitmap ^ bmp = gcnew Bitmap(320, 240);
				 pictureBox1->Width::set(320);
				 pictureBox1->Height::set(240);


				 // 8x16 MS Gothic
				 //DrawFont816(bmp);

				 // 11x23 굴림
				 //DrawFont1123(bmp);

				 // 16x16 굴림(13pt)
				 DrawGulimBold1616Start(bmp);

				 // 16x16 icon
				 DrawIcon1616(bmp);


			 }
			 void DrawIcon1616(Bitmap^ bmp)
			 {
				 int startX = 10;
				 int startY = 10;
				 int endX = 200;


				 int isInterval = 0;
				 for(int j=0;j<13;j++)
				 {
					 if(j>3)
						 isInterval = 2;

					 DrawIcon1616Height(bmp, startX + ((16 + isInterval)*j), startY, &icon16x16[j*32]); 
				 }
			 }
			 void DrawIcon1616Height(Bitmap^ bmp, int startX, int startY, unsigned short val[])
			 {
				 for(int i=0;i<16;i++)
				 {
					 DrawIcon1616Width(bmp, startX, startY+i, val[2*i] << 8 | val[2*i+1]);
				 }
			 }
			 void DrawIcon1616Width(Bitmap^ bmp, int startX, int startY, unsigned short val)
			 {
				 Color rColor = Color::White;

				 for(int i=0;i<16;i++)
				 {
					 if((val & 0x8000) == 0x8000)
					 {
						 bmp->SetPixel(startX+i, startY, rColor);
						 
					 }

					 val <<= 1;	 

				 }
				 pictureBox1->Image = bmp;
			 }


			 // font816.h 사용
			 void DrawBase(Bitmap^ bmp)
			 {
				 String^ fontStr = fontData->Text;

				 int textLength = fontData->Text->Length;

				 pat = toStandardString(fontStr);

				 const char* fonts = pat.c_str();


				 int sX = 0;
				 int sY = 0;
				 for(int i=0;i<textLength;i++)
				 {
					 DrawFont(bmp, sX, sY, font[fonts[i]]);

					 sX += 8;

				 }
			 }
			 void DrawFont(Bitmap^ bmp, int startX, int startY, UINT64 val)
			 {
				 Color rColor = Color::White;

				 for(int j=8-1;j>=0;j--)
				 {
					 // val의 마지막 2바이트 잘라와서
					 int mask = val & 0xff;

					 for(int i=8-1;i>=0;i--)
					 {
						 // 1bit 씩 비교
						 if((mask & 0x00000001) == 1)
							 bmp->SetPixel(startX+i, startY+j, rColor);

						 // 비교 후 mask 시프트
						 mask >>= 1;
					 }

					 // val 시프트
					 val >>= 8;

				 }
				 pictureBox1->Image = bmp;

			 }

			 
			 // msgothic816.h 사용
			 void DrawFont816(Bitmap^ bmp)
			 {
				 String^ fontStr = fontData->Text;

				 int textLength = fontData->Text->Length;

				 pat = toStandardString(fontStr);

				 const char* fonts = pat.c_str();

				 for(int j=0;j<textLength;j++)
				 {
					 DrawFont816Height(bmp, 130 + (8*j), 180, msgodic8x16[fonts[j]]);
				 }
			 }
			 void DrawFont816Height(Bitmap^ bmp, int startX, int startY, const unsigned short val[16])
			 {
				 for(int i=0;i<16;i++)
				 {
					 DrawFont816Width(bmp, startX, startY+i, val[i]);
				 }

			 }


			 void DrawFont816Width(Bitmap^ bmp, int startX, int startY, unsigned short val)
			 {
				 Color rColor = Color::White;

				 for(int i=8-1;i>=0;i--)
				 {
					 if((val & 0x80) == 0x80)
						 bmp->SetPixel(startX+i, startY, rColor);

					 val <<= 1;

				 }

				 pictureBox1->Image = bmp;
			 }

			 
			 // 1123Font.h 사용
#define FONTSTARTX 130	// 시작점x
#define FONTSTARTY 180	// 시작점y
#define FONTWIDTH 11	// 폰트width
#define FONTHEIGHT 23	// 폰트height
#define BITCOUNTER 24	// 3바이트로 한 줄을 표현
#define FONTDATASIZE 34	// 폰트dataSize
			 void DrawFont1123(Bitmap^ bmp)
			 {
				 String^ fontStr = fontData->Text;

				 int textLength = fontData->Text->Length;

				 pat = toStandardString(fontStr);

				 const char* fonts = pat.c_str();

				 for(int j=0;j<textLength;j++)
				 {
					 DrawFont1123Gulim(bmp, FONTSTARTX + (12*j), FONTSTARTY, &gulim11x23[(fonts[j]- 32) * FONTDATASIZE]); // space 부터 시작 [32]
				 }
			 }
			 // 11x23 데이터구조
			 // 가로로 그리지 않고 좌측상단부터 세로로 그린다 ↓↓↓↓↓↓↓↓↓↓↓ 순서
			 // 0x0B, 0xFC, 0xFF, 0x0F, 0xFC, 0xFF, 0x0F, 0xE0, 0x03, 0x00, 
			 // 0x00, 0x1F, 0x00, 0x00, 0xF8, 0x00, 0x00, 0xC0, 0x07, 0x00, 
			 // 0xF8, 0x00, 0x00, 0x1F, 0x00, 0xE0, 0x03, 0x00, 0xFC, 0xFF, 
			 // 0x0F, 0xFC, 0xFF, 0x0F,  // Code for char M
			 // [0], 픽셀데이터가 들어있는 line 길이
			 // [1~3], 리틀엔디언으로 바꾸고(0xFC 0x0F 0xFF) LSB(가장 오른쪽 비트)부터 한비트씩 읽어서 상단부터 그림
			 void DrawFont1123Gulim(Bitmap^ bmp, int startX, int startY, unsigned short* val)
			 {
				 Color rColor = Color::White;

				 // 첫번째 데이터만큼의 line이 있다..'I' 같은경우 6번만 돌면 됨
				 const unsigned short* pixFirstData = val;

				 *val++;

				 for(int i=0;i<*pixFirstData;i++)
				 {
					 unsigned short* pixData1 = val;
					 *val++;
					 unsigned short* pixData2 = val;
					 *val++;
					 unsigned short* pixData3 = val;
					 *val++;

					 // 비트를 뒤집어서 시프트시킴
					 int heightPixel = *pixData1 << 0 | *pixData2 << 8 | *pixData3 << 16;

					 // 가장 마지막 비트로 1이면 pixel, 0이면 pass
					 for(int j=0;j<BITCOUNTER;j++)
					 {
						 if((heightPixel & 0x00000001) == 1)
						 {
							 bmp->SetPixel(startX+i, startY+j, rColor);
						 }

						 // 하나씩 시프트 시키면서 마지막비트 확인
						 heightPixel >>= 1;
					 }
				 }
				 pictureBox1->Image = bmp;

			 }


			 // gulimBold13pt.h 사용
			 void DrawGulimBold1616Start(Bitmap^ bmp)
			 {
				 String^ fontStr = fontData->Text;

				 int textLength = fontData->Text->Length;

				 pat = toStandardString(fontStr);

				 const char* fonts = pat.c_str();

				 int nextX = 0;
				 for(int j=0;j<textLength;j++)
				 {
					 //DrawGulimBold1616(bmp, FONTSTARTX + (16*j), FONTSTARTY, &gulimBold16x16[(fonts[j]- 32) * 33]); // space 부터 시작 [32]
					 nextX = DrawGulimBold1616(bmp, nextX, FONTSTARTY, &gulimBold16x16[(fonts[j]- 32) * 33]); // space 부터 시작 [32]

				 }
			 }
			 int DrawGulimBold1616(Bitmap^ bmp, int startX, int startY, unsigned short* val)
			 {
				 Color rColor = Color::White;

				 // 첫번째 데이터만큼의 line이 있다..'I' 같은경우 6번만 돌면 됨
				 const unsigned short* pixFirstData = val;

				 *val++;

				 for(int i=0;i<*pixFirstData;i++)
				 {
					 unsigned short* pixData1 = val;
					 *val++;
					 unsigned short* pixData2 = val;
					 *val++;

					 // 비트를 뒤집어서 시프트시킴
					 int heightPixel = *pixData1 << 0 | *pixData2 << 8;

					 // 가장 마지막 비트로 1이면 pixel, 0이면 pass
					 for(int j=0;j<16;j++)
					 {
						 if((heightPixel & 0x00000001) == 1)
						 {
							 bmp->SetPixel(startX+i, startY+j, rColor);
						 }

						 // 하나씩 시프트 시키면서 마지막비트 확인
						 heightPixel >>= 1;
					 }
				 }
				 pictureBox1->Image = bmp;

				 int retCoordX = startX + (*pixFirstData + 1);

				 return retCoordX;
			 }

	};
}


