
struct VS_INPUT
{
    float4 position : POSITION; // 정점 좌표
    float4 color : COLOR;       // 정점 색상
};

struct VS_OUTPUT
{
    float4 position : SV_POSITION; // 시스템 예약 시맨틱(화면 위치용)
    float4 color : COLOR;          // 픽셀 셰이더로 넘길 색상
};

// IA - VS(정점 단위) - RS(자동/보간) - PS - OM
/*
 * IA (정점 데이터)
   ↓
   VS (정점 셰이더: 좌표 변환, 데이터 전달)
   ↓
   RS (래스터라이저: 정점 → 픽셀)
   ↓
   PS (픽셀 셰이더: 최종 색상 계산)
   ↓
   OM (출력 병합기: 화면 버퍼에 출력)
*/

//
// 렌더링 파이프라인의 VS 단계
VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output;
    output.position = input.position;
    output.color = input.color;
    
    return output;
}

// RS 자동

// 렌더링 파이프라인의 PS 단계
float4 PS(VS_OUTPUT input) : SV_Target
{
    return input.color;
}