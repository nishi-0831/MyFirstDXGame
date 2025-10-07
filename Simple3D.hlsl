//������������������������������������������������������������������������������
// �e�N�X�`�����T���v���[�f�[�^�̃O���[�o���ϐ���`
//������������������������������������������������������������������������������
Texture2D g_texture : register(t0); //�e�N�X�`���[
SamplerState g_sampler : register(s0); //�T���v���[

//������������������������������������������������������������������������������
// �R���X�^���g�o�b�t�@
// DirectX �����瑗�M����Ă���A�|���S�����_�ȊO�̏����̒�`
//������������������������������������������������������������������������������
cbuffer global
{
    float4x4 matWVP; // ���[���h�E�r���[�E�v���W�F�N�V�����̍����s��
    float4x4 matW; //���[���h�s��
    float4x4 matNormalTrans;//�@�������[���h���W�n�̕����x�N�g���ɂ���s��
    float4 diffuse;
    bool materialFlag;
    float padding[3];
};

cbuffer light
{
    float4x4 lightRot;
};


//������������������������������������������������������������������������������
// ���_�V�F�[�_�[�o�́��s�N�Z���V�F�[�_�[���̓f�[�^�\����
//������������������������������������������������������������������������������
struct VS_OUT
{
                 //�Z�}���e�B�N�X
    float4 pos : SV_POSITION; //�ʒu
    float2 uv : TEXCOORD;     //UV���W
    float4 color : COLOR; //�F
};

//������������������������������������������������������������������������������
// ���_�V�F�[�_
//������������������������������������������������������������������������������
VS_OUT VS(float4 pos : POSITION, float4 uv : TEXCOORD,float4 normal : NORMAL )
{
	//�s�N�Z���V�F�[�_�[�֓n�����
    VS_OUT outData;

    //�@������]
    normal = mul(normal, matNormalTrans);
    normal.w = 0;
	//���[�J�����W�ɁA���[���h�E�r���[�E�v���W�F�N�V�����s���������
	//�X�N���[�����W�ɕϊ����A�s�N�Z���V�F�[�_�[��
    outData.pos = mul(pos, matWVP);
    outData.uv = uv;//UV���W�͂��̂܂�

    float4 light = float4(0, 0, -1, 0);
    light = normalize(-light);
    
    
    outData.color = dot(normalize(normal), -light);

	//�܂Ƃ߂ďo��
    return outData;
}

//������������������������������������������������������������������������������
// �s�N�Z���V�F�[�_
//������������������������������������������������������������������������������
float4 PS(VS_OUT inData) : SV_Target
{
    //�T���v���[��uv���W���Ƀe�N�X�`����F�X����Ă�
    //�������ɃT���v���[
    //��������uv���W
    float4 color;
    if (materialFlag == true)
    {
        color = g_texture.Sample(g_sampler, inData.uv) * inData.color;
    }
    else
    {
        color = diffuse;
        //color = float4(1, 1, 1, 1);
    }
    return color;
        //return g_texture.Sample(g_sampler, inData.uv) * inData.color;
}