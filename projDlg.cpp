
// projDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "proj.h"
#include "projDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CprojDlg 대화 상자



CprojDlg::CprojDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PROJ_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CprojDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IMGLIST, ListBox);
	DDX_Control(pDX, IDC_TEMPLE, m_pic_temple);
	DDX_Control(pDX, IDC_TEST, m_pic_test);
}

BEGIN_MESSAGE_MAP(CprojDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()

	ON_BN_CLICKED(ID_OPEN, &CprojDlg::OnBnClickedOpen)
	ON_LBN_DBLCLK(IDC_IMGLIST, &CprojDlg::OnLbnDblclkImglist)
	ON_BN_CLICKED(IDC_GRAYSCALE, &CprojDlg::OnBnClickedGrayscale)
	ON_BN_CLICKED(IDC_GAUSSIANBLUR, &CprojDlg::OnBnClickedGaussianblur)
	ON_BN_CLICKED(IDC_HISTOGRAMEQ, &CprojDlg::OnBnClickedHistogrameq)
	ON_BN_CLICKED(IDC_MORPHOLOGY, &CprojDlg::OnBnClickedMorphology)
	ON_BN_CLICKED(IDC_ALL, &CprojDlg::OnBnClickedAll)
	ON_BN_CLICKED(ID_CLOSE, &CprojDlg::OnBnClickedClose)
	ON_BN_CLICKED(IDC_MATCH, &CprojDlg::OnBnClickedMatch)
	ON_BN_CLICKED(ID_CLEAR, &CprojDlg::OnBnClickedClear)
END_MESSAGE_MAP()


// CprojDlg 메시지 처리기

BOOL CprojDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CprojDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}

	SetDlgItemText(IDC_BOARD, _T("PCB 검사 가이드\r\n"
		"1-OPEN → 폴더 선택\r\n"
		"2-더블클릭 → 이미지 로드\r\n"
		"3-전처리 버튼들 순서대로\r\n"
		"4-ALL → 전체 처리\r\n"
		"5-MATCH → 부품 검출"));
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CprojDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



// 폴더 열기 버튼 클릭 시 호출되는 함수
void CprojDlg::OnBnClickedOpen()
{
	ITEMIDLIST* pidlBrowes;         // 폴더 선택 결과를 담는 구조체 포인터
	WCHAR pszPathname[MAX_PATH];    // 선택된 폴더의 전체 경로를 저장할 배열

	BROWSEINFO BrInfo;              // 폴더 선택 대화상자 설정에 사용하는 구조체

	BrInfo.hwndOwner = GetSafeHwnd();   // 부모 윈도우 핸들 등록 (현재 다이얼로그)
	BrInfo.pidlRoot = NULL;             // 루트 폴더를 지정하지 않음 (기본은 "내 컴퓨터")

	memset(&BrInfo, 0x00, sizeof(BrInfo));  // 구조체를 0으로 초기화
	BrInfo.pszDisplayName = pszPathname;    // 선택된 폴더명이 저장될 버퍼
	BrInfo.ulFlags = BIF_RETURNONLYFSDIRS;  // 실제 파일 시스템 폴더만 선택 가능 (가상 폴더 제외)

	BrInfo.lpfn = BrowseCallbackProc;       // 콜백 함수 포인터 (초기 위치 설정 등에 사용 가능)

	CString strInitPath = _T("C:\\");       // 폴더 대화상자 초기 경로 설정
	BrInfo.lParam = (LPARAM)strInitPath.GetBuffer();  // 초기 경로를 콜백에 전달

	// 폴더 선택 대화상자 실행
	pidlBrowes = ::SHBrowseForFolder(&BrInfo);

	// 사용자가 폴더를 선택했을 경우
	if (pidlBrowes != NULL) {
		SHGetPathFromIDList(pidlBrowes, pszPathname);  // ITEMIDLIST → 실제 경로 문자열로 변환

		CString folderPath(pszPathname);   // 예: "C:\\PCB_images"
		CString tpath;

		SetDlgItemText(IDC_PATH,folderPath);

		tpath.Format(_T("%s\\*.jpg"), folderPath);  // 선택 폴더 내의 모든 JPG 파일 검색 경로

		m_selectedFolder = folderPath;     // 멤버 변수에 선택된 폴더 경로 저장
		CFileFind finder;                  // 파일 탐색용 CFileFind 객체
		BOOL bWorking = finder.FindFile(tpath);  // 첫 파일 검색 시작
		ListBox.ResetContent();            // 리스트박스 내용 초기화
		CString fileName, DirName;

		// 폴더 내 모든 jpg 파일 순회
		while (bWorking) {
			bWorking = finder.FindNextFile();  // 다음 파일 검색

			if (finder.IsDirectory() || finder.IsDots())	// 폴더나 . .. 는 건너뛰기
				continue;

			fileName = finder.GetFileTitle();        // 확장자 제외한 파일명만 추출

			ListBox.AddString(fileName);             // 리스트박스에 파일명 추가
		}
	}
}

// 리스트박스에서 이미지 이름을 더블클릭했을 때 호출되는 함수
void CprojDlg::OnLbnDblclkImglist(){	
	// 기존 전처리 결과 초기화 (새로운 이미지 로드시 기존 결과 삭제)
	if (!m_processedTemplate.empty() || !m_processedTest.empty()) {
		m_processedTemplate.release();
		m_processedTest.release();
	}

	CString fileName;
	int index = ListBox.GetCurSel();     // 현재 선택된 항목의 인덱스를 가져옴

	// 짝수(템플릿), 홀수(테스트) 파일이 짝으로 구성되어 있을 경우
	// 홀수를 클릭했더라도 항상 짝수부터 로드하도록 인덱스 조정
	if (index % 2 == 1)
		index -= 1;
	if (index + 1 >= ListBox.GetCount()) return;
	CString templateName, testName;
	ListBox.GetText(index, templateName);      // 선택된 항목의 파일명 (템플릿)
	ListBox.GetText(index + 1, testName);      // 바로 다음 항목의 파일명 (테스트)

	imgLoad(templateName); // 첫 번째 이미지 로드 (템플릿)
	imgLoad(testName);     // 두 번째 이미지 로드 (테스트) 
}


//-------------------------------------------------------------
// 그레이스케일 버튼 클릭 핸들러
// 양쪽 이미지에 Grayscale 변환 적용
// 전처리 결과 Picture Control에 표시
//-------------------------------------------------------------
void CprojDlg::OnBnClickedGrayscale()
{
	if (temple_Mat.empty() || test_Mat.empty()) {
		SetDlgItemText(IDC_BOARD, _T("먼저 이미지를 더블클릭해서 로드하세요\n"));
		return;
	}
	// 템플릿과 테스트 이미지 모두 Grayscale 변환
	m_processedTemplate = m_preprocessor.grayScale(temple_Mat);
	m_processedTest = m_preprocessor.grayScale(test_Mat);

	DrawMat(m_processedTest, m_pic_test);
	SetDlgItemText(IDC_BOARD, _T("그레이 스케일 완료\n"));
}


//-------------------------------------------------------------
// 가우시안 블러 버튼 클릭 핸들러
// ksize=5로 노이즈 제거
// Grayscale 이후 단계에서만 실행 가능
//-------------------------------------------------------------
void CprojDlg::OnBnClickedGaussianblur(){
	if (m_processedTest.empty()) {
		SetDlgItemText(IDC_BOARD, _T("먼저 그레이스케일 버튼을 누르세요!\\n"));
		return;
	}

	// 양쪽 이미지에 동일한 블러 적용
	m_processedTemplate = m_preprocessor.gaussianBlur(m_processedTemplate);
	m_processedTest = m_preprocessor.gaussianBlur(m_processedTest, 5);

	DrawMat(m_processedTest, m_pic_test);
	SetDlgItemText(IDC_BOARD, _T("가우시안 블러 완료\n"));
}

//-------------------------------------------------------------
// 히스토그램 평활화 버튼 클릭 핸들러
// CLAHE 적용으로 대비 향상
//-------------------------------------------------------------
void CprojDlg::OnBnClickedHistogrameq()
{
	if (m_processedTest.empty()) {
		SetDlgItemText(IDC_BOARD, _T("먼저 그레이스케일 버튼을 누르세요!\n"));
		return;
	}
	m_processedTemplate = m_preprocessor.histogramEq(m_processedTemplate);
	m_processedTest = m_preprocessor.histogramEq(m_processedTest);
	DrawMat(m_processedTest, m_pic_test);

	SetDlgItemText(IDC_BOARD, _T("히스토그램 평탄화 완료\n"));

}

//-------------------------------------------------------------
// 모폴로지 연산 버튼 클릭 핸들러
// Morphological Opening으로 작은 노이즈 제거
//-------------------------------------------------------------
void CprojDlg::OnBnClickedMorphology()
{
	if (m_processedTest.empty()) {
		SetDlgItemText(IDC_BOARD, _T("먼저 그레이스케일 버튼을 누르세요!\n"));
		return;
	}
	m_processedTemplate = m_preprocessor.morphologyEx(m_processedTemplate);
	m_processedTest = m_preprocessor.morphologyEx(m_processedTest);
	DrawMat(m_processedTest, m_pic_test);

	SetDlgItemText(IDC_BOARD, _T("모폴로지  완료\n"));
}

//-------------------------------------------------------------
// 전체 전처리 버튼 클릭 핸들러
// 모든 전처리를 순차적으로 일괄 실행
// Grayscale이 없으면 자동으로 먼저 실행
//-------------------------------------------------------------
void CprojDlg::OnBnClickedAll()
{
	if (temple_Mat.empty() || test_Mat.empty()) {
		AfxMessageBox(_T("먼저 이미지를 더블클릭해서 로드하세요!"));
		return;
	}
	if (m_processedTest.empty()) {
		// Grayscale 전처리가 없으면 먼저 실행
		m_processedTemplate = m_preprocessor.grayScale(temple_Mat);
		m_processedTest = m_preprocessor.grayScale(test_Mat);
	}
	m_processedTemplate = m_preprocessor.all(m_processedTemplate);
	m_processedTest = m_preprocessor.all(m_processedTest);
	DrawMat(m_processedTest, m_pic_test);


	SetDlgItemText(IDC_BOARD, _T("모든 전처리 완료\n"));
}

//-------------------------------------------------------------
// 템플릿 매칭 버튼 클릭 핸들러
// 전처리된 이미지로 차이 검출 수행
// 검출된 결함 개수 UI에 표시
//-------------------------------------------------------------
void CprojDlg::OnBnClickedMatch()
{
	if (m_processedTemplate.empty() || m_processedTest.empty()) {
		SetDlgItemText(IDC_BOARD, _T("전처리 먼저 해주세요!\n"));
		return;
	}
	// 매칭 실행 및 바운딩 박스 결과 획득
	boundingImg = m_match.matchTemple(m_processedTemplate, m_processedTest);
	DrawMat(boundingImg, m_pic_test);

	// 검출된 결함 개수 표시
	int count = m_match.getDetectedCount();

	CString status;
	status.Format(_T("템플릿 매칭완료 \n""검출된 결함 : %d개"), count);
	SetDlgItemText(IDC_BOARD, status);
}
//-------------------------------------------------------------
// 프로그램 종료 버튼
// 모든 cv::Mat 메모리 해제 후 메인 윈도우 종료
//-------------------------------------------------------------
void CprojDlg::OnBnClickedClose()
{
	
	temple_Mat.release();
	test_Mat.release();
	m_processedTemplate.release();
	m_processedTest.release();
	boundingImg.release();
	AfxGetMainWnd()->PostMessage(WM_CLOSE);
}

//-------------------------------------------------------------
// 결과 초기화 버튼
// 전처리 결과만 해제하고 원본 이미지만 표시
//-------------------------------------------------------------
void CprojDlg::OnBnClickedClear()
{
	if (temple_Mat.empty() || test_Mat.empty()) {
		AfxMessageBox(_T("먼저 이미지를 더블클릭해서 로드하세요!"));
		return;
	}
	if (m_processedTemplate.empty() || m_processedTest.empty()) {
		AfxMessageBox(_T("전처리된 이미지가 없습니다 ."));
		return;
	}
	m_processedTemplate.release();
	m_processedTest.release();
	boundingImg.release();
	SetDlgItemText(IDC_BOARD, _T("초기화 완료\n"));

	DrawMat(test_Mat, m_pic_test);
	DrawMat(temple_Mat, m_pic_temple);
}

