#include "pch.h"
#include "match.h"
//-------------------------------------------------------------
// 템플릿 매칭 기반 차이 검출
// 기준 이미지와 테스트 이미지 비교하여 불일치 영역 검출
// PCB 불량 검출, 제품 외관 검사에 최적화
//-------------------------------------------------------------
cv::Mat Match::matchTemple(const cv::Mat& tempImg, const cv::Mat& testImg) {
    // 입력 이미지 유효성 검사
    // 비어있는 이미지는 MFC 메시지박스로 사용자에게 알림
    if (tempImg.empty() || testImg.empty()) {
        AfxMessageBox(_T("이미지 비어있음"));
        return cv::Mat();
    }

    // 결과 이미지 초기화 (testImg 복사본)
    cv::Mat result = testImg.clone();

    // 두 이미지의 픽셀별 절대 차이 계산
    cv::Mat diff;
    cv::absdiff(tempImg, testImg, diff);

    // 차이 이미지를 그레이스케일로 변환 (1채널 분석용)
    cv::cvtColor(diff, diff, cv::COLOR_BGR2GRAY);

    // 이진화: 차이가 30 이상인 픽셀만 흰색(255)으로 강조
    cv::Mat thresh;
    cv::threshold(diff, thresh, 30, 255, cv::THRESH_BINARY);

    // 5x5 사각형 커널로 Morphological Opening 적용
    // 작은 노이즈 제거를 위한 침식 → 팽창
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));
    cv::morphologyEx(thresh, thresh, cv::MORPH_OPEN, kernel);

    // 이진 이미지에서 외곽선 검출
    // RETR_EXTERNAL: 최상위 외곽선만 추출
    // CHAIN_APPROX_SIMPLE: 압축된 점 좌표만 저장
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(thresh, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    // 검출된 영역 카운터
    int count = 0;

    // 각 외곽선 분석
    for (const auto& contour : contours) {
        // 외곽선 면적 계산
        double area = cv::contourArea(contour);

        // 면적이 90 이상인 영역만 유효한 결함으로 판단
        if (area > 90) {
            // 외곽선을 감싸는 최소 사각형 계산
            cv::Rect bbox = cv::boundingRect(contour);

            // 결과 이미지에 빨간색 바운딩 박스 그리기
            cv::rectangle(result, bbox, cv::Scalar(0, 0, 255), 3);
            count++;
        }
    }

    // 멤버 변수에 최종 검출 개수 저장
    m_detectedCount = count;
    return result;
}

