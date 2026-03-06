#pragma once
#include <opencv2/opencv.hpp>

// 역할 : 두 이미지(템플릿 vs 테스트)를 비교하여 차이가 있는 영역을 검출하고,
// 검출된 영역을 빨간색 바운딩 박스로 표시하는 영상 매칭 엔진 클래스.
class Match {
private:
    int m_detectedCount = 0;  // 검출된 차이 영역(바운딩 박스)의 총 개수
                               // matchTemple() 실행 후 자동 업데이트

public:
    int getDetectedCount() { return m_detectedCount; }   //  검출된 이상 영역 개수를 반환

    //---------------------------------------------------------
    // 핵심 매칭 함수
    // **입력**:
    //   - tempImg: 기준(템플릿) 이미지 - cv::Mat (BGR 컬러 이미지)
    //   - testImg: 비교 대상(테스트) 이미지 - cv::Mat (BGR 컬러 이미지)
    //     * 주의: 두 이미지의 크기와 채널이 동일해야 함
    //
    // **처리 과정**:
    //   1. absdiff()로 픽셀 단위 절대 차이 계산
    //   2. 그레이스케일 변환 후 threshold 이진화(30 이상)
    //   3. Morphology Opening으로 노이즈 제거
    //   4. findContours로 차이 영역 외곽선 검출
    //   5. 면적 100 이상인 영역만 빨간 바운딩 박스로 표시
    //
    // **출력**:
    //   - testImg 복사본에 빨간 바운딩 박스가 그려진 cv::Mat 반환
    //   - m_detectedCount에 실제 검출된 영역 개수 저장
    //
    // **반환값**:
    //   - 성공: 처리된 이미지 (testImg + 빨간 박스들)
    //   - 실패: 빈 Mat (입력 이미지 중 하나라도 empty인 경우)
    //---------------------------------------------------------
    cv::Mat matchTemple(const cv::Mat& tempImg, const cv::Mat& testtImg);
};
