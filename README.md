# The ideal clock

理想的な置き時計を求めて

## 要件と設計

### 状態

- 表示モード
  - hh:mm
  - hh:mm:ss
  - hh:mm:ss.ff
- 秒点滅
  - 点灯
  - 消灯
  - 点滅

- 内部ステータス
  - Wi-Fi接続フラグ
    - CONNECTED(接続済)
    - NON_CONNECTION(未接続)
  - ネットワークステータス
    - INTERNET_OK(外に出られる)
    - IP_OK(有効なIPアドレスが割り当てられた)
    - UNKNOWN(未接続・IPなし・不明)

  - 時刻同期状態
    - NTP_SYNCHRONIZED
      - 最終同期からn時間後に`INTERNAL_CLOCK`へ遷移
    - INTERNAL_CLOCK

#### 課題

名前解決どうするん？

何をもって`INTERNET_OK`にするか
`8.8.8.8`にping通す？？

コンフィグどこに保存？


### スレッド

-  7セグ時刻表示
  - 時刻取得
  - 時刻表示
    - 1桁目表示
      - カソード駆動
      - 7セグドライバ転送
- キャラクタOLED
- 時刻同期
