PCB 불량 검출 시스템 v1.0  
OpenCV + MFC 기반 템플릿 매칭 프로토타입  

기능 개요  
이미지 로드: 폴더 단위 템플릿/테스트 이미지 쌍 로드  
전처리: Grayscale → Gaussian Blur → CLAHE → Morphology Opening  
템플릿 매칭: 픽셀 차이 기반 결함 검출 + 빨간 바운딩 박스  
결함 카운트: 실시간 검출 개수 표시  
UI:  순차 버튼 + 상태 메시지  

기술 스택  
Frontend: MFC Dialog + Picture Control (C++)  
Backend: OpenCV 4._ (C++17)  
전처리: CLAHE(1.5,8x8), Morphological Opening(3x3 Ellipse)  
매칭: absdiff + Threshold(30) + Contour Detection(area>90)  
빌드: Visual Studio 2019+  

성능  
결함 검출 파이프라인:  
absdiff → GRAY → Threshold(30) → Open(5x5) → Contours → BoundingBox  

전처리 효과:  
- Grayscale: 색상 변화 무시  
- CLAHE: 조명 불균일 보정 
- Morphology: 노이즈 제거  

현재 한계점  
매직넘버: threshold=30, area=120 하드코딩  
회전/크기 변화: 템플릿 매칭 한계  
배치 처리: 수동 1개씩 처리  

개발 로드맵  
v1.0 - 템플릿 매칭 프로토타입  
v2.0 - YOLOv3 통합   
v3.0 - 실시간 카메라 연동  

