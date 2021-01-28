/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
* other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
* EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
* SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS
* SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
* this software. By using this software, you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2019 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
#include "task_function.h"
/* Start user code for import. Do not edit comment generated here */
#include "platform.h"
#include "r_gpio_rx_if.h"
#include "r_mpc_rx_if.h"
#include "r_cmt_rx_if.h"
#include "r_s12ad_rx_if.h"
#include "r_dmaca_rx_if.h"
#include "string.h"

void IO_ADC_Init(void);
void IO_MTU7_Init(void);
void IO_DMA_Init(void);
void IO_DMA0_Start(void);

static uint16_t						g_ADC[8];
static dmaca_transfer_data_cfg_t 	g_tDmacaCfg;


/* End user code. Do not edit comment generated here */

void Task_AppMain(void * pvParameters)
{
/* Start user code for function. Do not edit comment generated here */

	// DMA初期化
	IO_DMA_Init();

	// ADC初期化処理
	IO_ADC_Init();

	// MTU7設定
	IO_MTU7_Init();

	//
	IO_DMA0_Start();

	while(1)
	{
		printf("ADC00:%d, ADC01:%d, ADC02:%d, ADC03:%d, ADC04:%d, ADC05:%d, ADC06:%d, ADC07:%d\n",
					g_ADC[0], g_ADC[1], g_ADC[2], g_ADC[3], g_ADC[4], g_ADC[5], g_ADC[6], g_ADC[7]);
		vTaskDelay(1000);

	}
/* End user code. Do not edit comment generated here */
}
/* Start user code for other. Do not edit comment generated here */


const gpio_port_pin_t		g_AdcPinInfo[] =
{
	GPIO_PORT_4_PIN_0,
	GPIO_PORT_4_PIN_1,
	GPIO_PORT_4_PIN_2,
	GPIO_PORT_4_PIN_3,
	GPIO_PORT_4_PIN_4,
	GPIO_PORT_4_PIN_5,
	GPIO_PORT_4_PIN_6,
	GPIO_PORT_4_PIN_7,
};


//=================================================================================================
// ADCコールバック
//=================================================================================================
void IO_ADC_Callback(void *pArgs)
{
//	printf("ADC00:%d, ADC01:%d, ADC02:%d, ADC03:%d, ADC04:%d, ADC05:%d, ADC06:%d, ADC07:%d\n",
//			S12AD.ADDR0, S12AD.ADDR1, S12AD.ADDR2, S12AD.ADDR3, S12AD.ADDR4, S12AD.ADDR5, S12AD.ADDR6, S12AD.ADDR7 );
}

//=================================================================================================
// ADC初期化処理
//=================================================================================================
void IO_ADC_Init(void)
{
	adc_err_t			eAdcResult = ADC_SUCCESS;
	adc_cfg_t			tAdcCfg;
	adc_ch_cfg_t    	tChCfg;
	mpc_err_t			eMpcResult = MPC_SUCCESS;
	mpc_config_t		tMpcCfg;


	// A/Dコンバータ初期化（複数のチャネルを連続スキャンモードでA/D変換）
	memset(&tAdcCfg, 0x00, sizeof(tAdcCfg));
	tAdcCfg.resolution = ADC_RESOLUTION_12_BIT;				// 変換精度(12Bit精度でA/D変換を実施)
	tAdcCfg.alignment = ADC_ALIGN_RIGHT;					// データレジスタフォーマット(A/D変換結果を右詰めで格納)
	tAdcCfg.add_cnt = ADC_ADD_AVG_4_SAMPLES;				// 変換値加算/平均モード(4回変換の平均値を使用)
//	tAdcCfg.add_cnt = ADC_ADD_AVG_2_SAMPLES;				// 変換値加算/平均モード(2回変換の平均値を使用)
//	tAdcCfg.add_cnt = ADC_ADD_OFF;							// 変換値加算/平均モード(加算/平均モードを使用しない)
	tAdcCfg.clearing = ADC_CLEAR_AFTER_READ_OFF;			// A/Dデータレジスタ自動クリア(A/Dデータレジスタを自動クリアしない)
	tAdcCfg.trigger = ADC_TRIG_SYNC_TRG7AN_OR_UDF7N;		// 変換起動トリガ(MTU7 TGRA)
	tAdcCfg.trigger_groupb = ADC_TRIG_NONE;					// グループBのA/D変換開始トリガ(なし)
	tAdcCfg.trigger_groupc = ADC_TRIG_NONE;					// グループCのA/D変換開始トリガ(なし)
	tAdcCfg.priority = 3;									// S12ADIn割り込み優先順位[0-15] ※0を指定した場合はS12ADIn割り込みは禁止される
	tAdcCfg.priority_groupb = 0;							// S12GBADIn・GBADIn割り込み優先順位[0-15] ※0を指定した場合はS12GBADIn・GBADIn割り込みは禁止される
	tAdcCfg.priority_groupc = 0;							// S12GCADIn割り込み優先順位[0-15] ※0を指定した場合はS12GCADIn割り込みは禁止される
	tAdcCfg.temp_sensor = ADC_TEMP_SENSOR_NOT_AD_CONVERTED;	// 温度センサ(温度センサ出力のA/D変換を行わない)
	tAdcCfg.add_temp_sensor = ADC_TEMP_SENSOR_ADD_OFF;		// 温度センサの変換値加算/平均モード(温度センサの加算/平均モードを使用しない)
	eAdcResult = R_ADC_Open(0, ADC_MODE_CONT_MULTI_CH, &tAdcCfg, IO_ADC_Callback);
	if (eAdcResult != ADC_SUCCESS)
	{
		while(1);
	}

	// A/D変換を行う端子を設定する
	memset(&tMpcCfg, 0x00, sizeof(tMpcCfg));
	tMpcCfg.analog_enable = true;
	tMpcCfg.irq_enable = false;
	tMpcCfg.pin_function = 0;
	for(uint8_t i = 0 ; i < (sizeof(g_AdcPinInfo) / sizeof(gpio_port_pin_t)) ; i++)
	{
		// 端子を入力に設定
		R_GPIO_PinDirectionSet(g_AdcPinInfo[i], GPIO_DIRECTION_INPUT);

		// 端子をアナログ端子として使用するように設定
		eMpcResult = R_MPC_Write(g_AdcPinInfo[i], &tMpcCfg);
		if (eMpcResult != MPC_SUCCESS)
		{
			while(1);
		}
	}

	// A/D変換を行うチャネルを設定
	memset(&tChCfg, 0x00, sizeof(tChCfg));
	tChCfg.chan_mask = ADC_MASK_CH0 | ADC_MASK_CH1 | ADC_MASK_CH2 | ADC_MASK_CH3 |		// 使用するチャネルを選択(Ch0 - Ch7)
					   ADC_MASK_CH4 | ADC_MASK_CH5 | ADC_MASK_CH6 | ADC_MASK_CH7 ;
	tChCfg.chan_mask_groupb = ADC_MASK_GROUPB_OFF;										// グループBで使用するチェネルを選択(使用しない)
	tChCfg.chan_mask_groupc = ADC_MASK_GROUPC_OFF;										// グループCで使用するチェネルを選択(使用しない)
	tChCfg.priority_groupa = ADC_GRPA_PRIORITY_OFF;										// グループ優先制御動作の設定(使用しない)
	tChCfg.add_mask = ADC_MASK_CH0 | ADC_MASK_CH1 | ADC_MASK_CH2 | ADC_MASK_CH3 |		// 加算モードを使用するチャネルを選択(Ch0 - Ch7)
			          ADC_MASK_CH4 | ADC_MASK_CH5 | ADC_MASK_CH6 | ADC_MASK_CH7 ;
	tChCfg.diag_method = ADC_DIAG_OFF;													// 自己診断モード(OFF)
	tChCfg.anex_enable = false;															// 拡張アナログ入力の使用有無(使用しない)
	tChCfg.sample_hold_mask = ADC_MASK_SAMPLE_HOLD_OFF;									// サンプル&ホールド回路を使用するチェネル(OFF)
	tChCfg.sample_hold_states = ADC_SST_SH_CNT_DEFAULT;									// サンプリング時間(デフォルト)
	eAdcResult = R_ADC_Control(0, ADC_CMD_ENABLE_CHANS, &tChCfg);
	if (eAdcResult != ADC_SUCCESS)
	{
		while(1);
	}

	eAdcResult = R_ADC_Control(0, ADC_CMD_ENABLE_TRIG, NULL);
	if (eAdcResult != ADC_SUCCESS)
	{
		while(1);
	}
}


//================================================================================================
// MTU7設定
//================================================================================================
void IO_MTU7_Init(void)
{
	// マルチファンクションタイマーパルスユニットモジュールストップ状態解除
	SYSTEM.PRCR.WORD = 0xA502;
	MSTP(MTU7) = 0;
	SYSTEM.PRCR.WORD = 0xA500;

	// MTU7  タイマコントロールレジスタ
	MTU7.TCR.BIT.TPSC = 3;					// タイマプリスケーラ選択ビット(TPSC)					: PCLKA/1でカウント
	MTU7.TCR.BIT.CKEG = 0;					// クロックエッジ選択ビット(CKEG)					: 立ち上がりでエッジカウント
	MTU7.TCR.BIT.CCLR = 1;					// カウンタクリア要因選択ビット(CCLR)					: TGRAのコンペアマッチでTCNTクリア

	// MTU7タイマモードレジスタ
	MTU7.TMDR1.BIT.BFA = 0;					// バッファ動作Aビット(BFA)						: MTU0.TGRBとMTU0.TGRDは通常動作
	MTU7.TMDR1.BIT.BFB = 0;					// バッファ動作Bビット(BFB)						: MTU0.TGRBとMTU0.TGRDは通常動作
	MTU7.TMDR1.BIT.MD = 0;					// モード選択ビット(MD)							: ノーマルモード

	// MTU7 タイマI/Oコントロールレジスタ
	MTU7.TIORH.BIT.IOA = 0;					// I/OコントロールAビット(IOA)						: 出力禁止
	MTU7.TIORH.BIT.IOB = 0;					// I/OコントロールBビット(IOB)						: 出力禁止
	MTU7.TIORL.BIT.IOC = 0;					// I/OコントロールCビット(IOC)						: 出力禁止
	MTU7.TIORL.BIT.IOD = 0;					// I/OコントロールDビット(IOD)						: 出力禁止

	MTU7.TCNT	= 0x0000;
	MTU7.TGRA	= 60000;

	// MTU7 タイマインタラプトイネーブルレジスタ
	MTU7.TIER.BIT.TTGE = 1;					// A/D変換開始要求許可ビット(TTGE)				: A/D変換開始要求の発生を許可
	MTU7.TIER.BIT.TTGE2 = 0;				// A/D変換開始要求許可2ビット(TTGE2)				: MTUn.TCNTのアンダーフロー（谷)によるA/D変換要求を禁止
	MTU7.TIER.BIT.TCIEV = 0;				// オーバーフロー割込み許可ビット(TCIEV)				: 割込み要求(TCIV)を禁止
	MTU7.TIER.BIT.TGIED = 0;				// TGR割り込み許可Dビット(TGIED)					: 割込み要求(TCID)を禁止
	MTU7.TIER.BIT.TGIEC = 0;				// TGR割り込み許可Cビット(TGIEC)					: 割込み要求(TCIC)を禁止
	MTU7.TIER.BIT.TGIEB = 0;				// TGR割り込み許可Bビット(TGIEB)					: 割込み要求(TCIB)を禁止
	MTU7.TIER.BIT.TGIEA = 0;				// TGR割り込み許可Aビット(TGIEA)					: 割込み要求(TCIA)を禁止

	// MTU7 カウント開始
	MTU.TSTRB.BIT.CST7 = 1;
}


//=================================================================================================
// DMA0コールバック処理
//=================================================================================================
void IO_DMA_Callback(void)
{
	dmaca_return_t				tDmacaResult = DMACA_SUCCESS;
	dmaca_stat_t				tDmacaStatus;


	tDmacaResult = R_DMACA_Control(DMACA_CH0, DMACA_CMD_STATUS_GET, &tDmacaStatus);
	if (tDmacaResult == DMACA_SUCCESS)
	{
		if (tDmacaStatus.esif_stat == false)
		{
			// 転送終了割込みフラグをクリア
			tDmacaResult = R_DMACA_Control(DMACA_CH0, DMACA_CMD_DTIF_STATUS_CLR, NULL);

			IO_DMA0_Start();
		}
		else if (tDmacaStatus.dtif_stat == true)
		{
			// 転送エスケープ終了割込みフラグをクリア
			tDmacaResult = R_DMACA_Control(DMACA_CH0, DMACA_CMD_ESIF_STATUS_CLR, NULL);

			// 転送終了割込みフラグをクリア
			tDmacaResult = R_DMACA_Control(DMACA_CH0, DMACA_CMD_DTIF_STATUS_CLR, NULL);

			IO_DMA0_Start();
		}
		else
		{
			// DMAC転送を再開
			tDmacaResult = R_DMACA_Control(DMACA_CH0, DMACA_CMD_RESUME, &tDmacaStatus);
		}
	}
}


//=================================================================================================
// DMA初期化
//=================================================================================================
void IO_DMA_Init(void)
{
	dmaca_return_t				tDmacaResult = DMACA_SUCCESS;


	// DMAC内部情報を初期化
	R_DMACA_Init();

	// DMA0のコールバック設定
	tDmacaResult = R_DMACA_Int_Callback(DMACA_CH0, IO_DMA_Callback);
	if (tDmacaResult != DMACA_SUCCESS)
	{
		while(1);
	}

	// DMA0割り込み許可
	tDmacaResult = R_DMACA_Int_Enable(DMACA_CH0, 3);
	if (tDmacaResult != DMACA_SUCCESS)
	{
		while(1);
	}

	// DMAC0のオープン
	tDmacaResult = R_DMACA_Open(DMACA_CH0);
	if (tDmacaResult != DMACA_SUCCESS)
	{
		while(1);
	}

	//　DMAC0を生成
	memset(&g_tDmacaCfg, 0x00, sizeof(g_tDmacaCfg));
	g_tDmacaCfg.transfer_mode = DMACA_TRANSFER_MODE_NORMAL;							// 転送モード							: ノーマル転送
	g_tDmacaCfg.repeat_block_side = DMACA_REPEAT_BLOCK_DISABLE;						// リピート/ブロックモードモードでのリピート領域	: 指定しない
	g_tDmacaCfg.data_size = DMACA_DATA_SIZE_WORD;									//　転送データサイズ
	g_tDmacaCfg.act_source = IR_S12ADC0_S12ADI0;									// DMAC起動要因割り込み				: ADC0変換完了割り込み
	g_tDmacaCfg.request_source = DMACA_TRANSFER_REQUEST_PERIPHERAL;					// DMACA転送要求元					: 周辺回路モジュール・外部割込み入力端子からの割り込み
	g_tDmacaCfg.dtie_request = DMACA_TRANSFER_END_INTERRUPT_ENABLE;					// 転送終了割込み要求					: 無効
	g_tDmacaCfg.esie_request = DMACA_TRANSFER_ESCAPE_END_INTERRUPT_DISABLE;			// 転送エスケープ終了割込み要求			: 有効
	g_tDmacaCfg.rptie_request = DMACA_REPEAT_SIZE_END_INTERRUPT_DISABLE;			// リピートサイズ割り込み要求				: 無効
	g_tDmacaCfg.sarie_request = DMACA_SRC_ADDR_EXT_REP_AREA_OVER_INTERRUPT_DISABLE;	// 転送元アドレスの拡張リピート領域オーバーフロー割込み	: 無効
	g_tDmacaCfg.darie_request = DMACA_DES_ADDR_EXT_REP_AREA_OVER_INTERRUPT_DISABLE;	// 転送先アドレスの拡張リピート領域オーバーフロー割込み	: 無効
	g_tDmacaCfg.src_addr_mode = DMACA_SRC_ADDR_INCR;								// 転送元アドレスモード					: インクリメント
	g_tDmacaCfg.src_addr_repeat_area = DMACA_SRC_ADDR_EXT_REP_AREA_NONE;			// 転送元アドレスの拡張リピート領域			: 未指定
	g_tDmacaCfg.des_addr_mode = DMACA_DES_ADDR_INCR;								// 転送先アドレスモード					: インクリメント
	g_tDmacaCfg.des_addr_repeat_area = DMACA_DES_ADDR_EXT_REP_AREA_NONE;			// 転送先アドレスの拡張リピート領域			: 未指定
	g_tDmacaCfg.offset_value = 0;													// DMAオフセットレジスタのオフセット			: 0(※DMAC0のみ有効)
	g_tDmacaCfg.interrupt_sel = DMACA_ISSUES_INTERRUPT_TO_CPU_END_OF_TRANSFER;		// 割り込み選択						:
	g_tDmacaCfg.p_src_addr = (void *)&S12AD.ADDR0;									// 転送元の開始アドレス
	g_tDmacaCfg.p_des_addr = (void *)g_ADC;											// 転送先の開始アドレス
	g_tDmacaCfg.transfer_count = 8;													// 転送回数
	g_tDmacaCfg.block_size = 0;														// リピート/ブロックサイズ
}

//=================================================================================================
// DMA開始
//=================================================================================================
void IO_DMA0_Start(void)
{
	dmaca_return_t				tDmacaResult = DMACA_SUCCESS;
	dmaca_stat_t				tDmacaStatus;

	// DMAC0のDMA転送を禁止する
	tDmacaResult = R_DMACA_Control(DMACA_CH0, DMACA_CMD_DISABLE, &tDmacaStatus);
	if (tDmacaResult != DMACA_SUCCESS)
	{
		return;
	}

	//18.2.7 DMA アドレスモードレジスタ(DMAMD)

	tDmacaResult = R_DMACA_Create(DMACA_CH0, &g_tDmacaCfg);
	if (tDmacaResult != DMACA_SUCCESS)
	{
		while(1);
	}

	// DMAC0のDMA転送を許可する
	tDmacaResult = R_DMACA_Control(DMACA_CH0, DMACA_CMD_ENABLE, &tDmacaStatus);
	if (tDmacaResult != DMACA_SUCCESS)
	{
		while(1);
	}
}

/* End user code. Do not edit comment generated here */
