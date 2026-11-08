#include <REGX52.H>

/*定義I2C之
Start condition (起始條件),
Stop condition  (終止條件),
Byte Write      (發送1個Byte),
Byte Read       (接收1個Byte),
Acknowledge     (發送應答),
ACK Detection   (接收應答)*/

sbit I2C_SCL=P2^1; //位聲明,電路圖上SCL接P2^1
sbit I2C_SDA=P2^0; //位聲明,電路圖上SDA接P2^0



//函式定義：
/**
  * @brief  I2C_起始條件
  * @param  無
  * @retval 無
  */
void I2C_Start(void) //Start condition
{
	I2C_SDA=1; //SDA可能是0也可能是1,I2C之基本時序不論哪一種,結束時SCL皆為0
	           //而我們在SCL為低位時可變化SDA數據,所以先將SDA拉至高位
	I2C_SCL=1; //接著將SCL拉高位(也可能SCL原本剛好就是高位)
	//也就是說當I2C_Start被呼叫當作一般起始條件,那SCL,SDA本來就是高位
	//但若I2C_Start被呼叫時是皆在複合格式之PA後,以上兩條可確保SCL,SDA皆為高位
	I2C_SDA=0; //如Start基本時序圖SCL高位時,SDA拉低
	I2C_SCL=0; //接著如Start基本時序圖SCL拉低
}



//函式定義：
/**
  * @brief  I2C_停止條件
  * @param  無
  * @retval 無
  */
void I2C_Stop(void) //Stop condition
{
	I2C_SDA=0; //I2C_Stop在發送及接收應答後,SDA可能為高or低位,所以先拉低
	//Stop condition本身並不要求SCL要從低位開始,不過此處確實6大基本時序
	//除了Stop condition外都是以SCL低位結束,因此確實能不寫I2C_SCL=0;
	I2C_SCL=1; //將SCL拉高再將SDA拉高才是形成Stop condition之條件
	I2C_SDA=1;
}
// 這樣寫就是確實讓SDA在SCL為高位時, 從低位拉高



//函式定義：
/**
  * @brief  I2C_傳送1Byte(Byte Write)
  * @param  欲傳送之1個Byte
  * @retval 無
  */
void I2C_SendByte(unsigned char Byte) //Byte Write
{
	unsigned char i;
	for(i=0;i<8;i++) //循環8次寫8位
	{
		I2C_SDA=Byte&(0x80>>i); 
		//Byte&0x80本身只輸出0x80or0x00,但SDA在此只可表示高低位(0or1)
		//將0x80or0x00指定給SDA會隱性轉型,且會遵守C語言裡非0即為真之規則
		//即0x80表1,0x40表1,0x70表1;只有0x00表示0
		
		//也可寫成I2C_SDA=(Byte&(0x80>>i))?1:0;
		//條件式 ? 條件成立時的結果 : 條件不成立時的結果;
		//(Byte&(0x80>>i))非0則SDA=1;(Byte&(0x80>>i))為0則SDA=0
		I2C_SCL=1;   //SCL拉高,讓從機讀數據,若從機Datasheet中讀數據之時間較長
		             //則有時下一行要寫延時函數
		I2C_SCL=0;   //SCL拉低,準備在讓MCU寫數據
	}
}



//函式定義：
/**
  * @brief  I2C_讀取1Byte(Byte Read)
  * @param  無
  * @retval MCU從從機讀取之1Byte
  */
unsigned char I2C_ReceiveByte(void) //Byte Read
{
	unsigned char i,Byte=0x00;      //Byte即後續回傳值
	I2C_SDA=1;       //因I2C之線路接法為開漏輸出(Open-Drain)配合上拉電組
					 //因此SDA=1相當於MCU釋放SDA,此時從機即可自由傳送0or1
	for(i=0;i<8;i++) 
	{
		I2C_SCL=1;   //接著將SCL拉高讀取SDA數據
		if(I2C_SDA){Byte|=(0x80>>i);} 
		//假設從機傳送Byte=0xA5=1010 0101
		//當i=0,從機傳送1,SDA讀取1,0x00|=0x80,則Byte結果為1000 0000
		//當i=1,從機傳送0,SDA讀取0,不執行bitwise,則Byte結果為1000 0000
		//當i=2,從機傳送1,SDA讀取1,0x80|=0x20,則Byte結果為1010 0000
		//當i=3,從機傳送0,SDA讀取0,不執行bitwise,則Byte結果為1010 0000
		//當i=4,從機傳送0,SDA讀取0,不執行bitwise,則Byte結果為1010 0000
		//	.....
		//當i=7,從機傳送1,SDA讀取1,0xA4|=0x01,則Byte結果為1010 0101
		I2C_SCL=0;
	}
	return Byte;
}

/*
Byte Write(MCU → Slave):MCU控制SDA，將資料逐Bit傳出

Byte Read (Slave → MCU):MCU釋放SDA，由Slave控制SDA，MCU負責讀取資料。
*/



//函式定義：
/**
  * @brief  I2C_傳送Ack(Acknowledge)
  * @param  AckBit(主機發送給從機之應答位,0表應答1表非應答)
  * @retval 無
  */
void I2C_SendAck(unsigned char AckBit) //Acknowledge(主機傳送ACK給從機)
	//Acknowledge時,AckBit只存1or0,0表應答1表非應答
{
	I2C_SDA=AckBit; //將SDA指定為應答位(AckBit為0表應答1表非應答)
	I2C_SCL=1;      //SCL置高讀取
	I2C_SCL=0;      //SCL置低
}



//函式定義：
/**
  * @brief  I2C_接收Ack(Acknowledge)
  * @param  無
  * @retval AckBit(從機發送給主機接收之應答位,0表應答1表非應答)
  */
unsigned char I2C_ReceiveAck(void)
{
	unsigned char AckBit=0; 
	//回傳AckBit=0表從機應答,回傳AckBit=1表從機非應答
	I2C_SDA=1;      //因I2C之線路接法為開漏輸出(Open-Drain)配合上拉電阻
				    //因此SDA=1相當於MCU釋放SDA,此時從機即可自由傳送0or1
	I2C_SCL=1;      //SCL置高讀取
	if(I2C_SDA)AckBit=1; //MCU釋放SDA後若從機回傳SDA非0,則將AckBit指定為1
	I2C_SCL=0;      //SCL置低
	return AckBit;  //回傳AckBit
}