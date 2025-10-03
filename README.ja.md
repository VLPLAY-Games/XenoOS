# XenoOS

- [Read in Russian](README.ru.md)
- [Read in English](README.md)
---
- XenoOS Core v0.2.3
- XenoOS Bootloader v0.2.1
- XenoOS Recovery v0.2.1
- XenoOS Software v0.2.3

## 概要

これは、ファイルシステム、Wi-Fi、SDカード、その他のシステム設定など、様々なシステムコンポーネントと対話するためのコマンドラインインターフェース（CLI）システムです。このシステムは、ファイル、ディレクトリ、システム設定を管理するための様々なコマンドをサポートしています。

このシステムはESP32ファミリーのマイクロコントローラー全体向けに設計されていますが、特にESP32-C3モデル向けに調整されています。

## 機能

- ファイル管理（例：ファイルの作成、削除、名前変更）
- システム診断、工場出荷時設定へのリセット、再インストール
- Wi-Fi接続管理
- SDカード操作
- コマンド履歴
- メモリとCPU情報

## コマンド一覧

このドキュメントには、利用可能なコマンドの一覧と簡単な説明、パラメータ、使用例が含まれています。

| コマンド   | 説明                      | パラメータ                                      | 使用例                       |
|-----------|----------------------------------|-------------------------------------------------|-------------------------------------|
| `wget`    | URLからファイルをダウンロード         | `<url> [<path>]`                                | `wget http://example.com/file.txt /path/to/save` |
| `help`    | ヘルプ情報を表示        | -                                               | `help`                              |
| `mkdir`   | 新しいディレクトリを作成           | `<path>`                                        | `mkdir /new_directory`             |
| `ls`      | ディレクトリの内容を一覧表示         | `<path>`                                        | `ls /sys`                           |
| `cd`      | 現在のディレクトリを変更    | `<path>`                                        | `cd /path/to/directory`            |
| `rm`      | ファイルまたはディレクトリを削除     | `<path>`                                        | `rm /path/to/file`                 |
| `wifi`    | Wi-Fi接続を管理         | `connect, disconnect, reconnect, scan, status, ip, help` | `wifi connect SSID password`  |
| `free`    | 利用可能なメモリを表示           | -                                               | `free`                              |
| `sdcard`  | SDカードを管理                  | `info, help`                                    | `sdcard info`                       |
| `clear`   | 画面をクリア                | -                                               | `clear`                             |
| `system`  | システムコマンド                 | `restart, info, update, diagnostic, freset, check, installer, help`       | `system restart`                   |
| `cpu`     | プロセッサ情報を表示   | `info, temp`                                    | `cpu info`                          |
| `cat`     | ファイルの内容を読み取り              | `<path_to_file>`                                | `cat /path/to/file`                |
| `rename`  | ファイルの名前を変更                    | `<old_file_path> <new_file_path>`               | `rename /path/oldname /path/newname` |
| `mac`     | MACアドレスを表示             | `info`                                          | `mac info`                          |
| `touch`   | 空のファイルを作成            | `<path_to_file>`                                | `touch /path/to/file`              |
| `mv`      | ファイルを移動                      | `<source_path> <destination_path>`              | `mv /path/to/source /path/to/destination` |
| `cp`      | ファイルをコピー                      | `<source_path> <destination_path>`              | `cp /path/to/source /path/to/destination` |
| `history` | コマンド履歴                 | `clear, read, help`                             | `history clear`                    |
| `hash`    | ハッシュ値を計算              | `<file_path>`                                   | `hash /path/file`                  |
| `ping`    | ホストまたはIPをping               | `<host_or_ip>`                                  | `ping 192.168.1.1`                 |
| `find`    | ファイルを検索                | `<path_to_search>`                              | `find /path/to/search`             |
| `df`      | ファイルシステム情報を表示  | `info`                                          | `df info`                          |
| `tar`     | アーカイブを操作              | `extract, help`                                 | `tar extract archive.tar`          |

## インストール

### ESP32-C3向け代替インストール方法
**ESP32-C3**を使用している場合、**esptool**または類似のツールを使用してシステムをインストールできます。以下の手順に従ってください：

1. GitHubの[リリース](https://github.com/VLPLAY-Games/XenoOS/releases)セクションに移動します。
2. 最新のファームウェアアーカイブをダウンロードします。
3. **統合ファームウェア**をフラッシュするか、各ファイルを個別にフラッシュできます：
   - 統合ファームウェアをフラッシュする場合：
     ```bash
     esptool.py --chip esp32c3 --baud 921600 write_flash 0x0 XenoOS.merged.bin
     ```
     複数のデバイスが接続されている場合は、ポートを明示的に指定します：
     ```bash
     esptool.py --chip esp32c3 --port /dev/ttyUSB0 --baud 921600 write_flash 0x0 XenoOS.merged.bin
     ```
   - 各ファイルを個別にフラッシュする場合：
     ```bash
     esptool.py --chip esp32c3 --baud 921600 write_flash \
       0x1000 XenoOS.bootloader.bin \
       0x8000 XenoOS.partitions.bin \
       0x10000 XenoOS.bin
     ```
     ここでも、複数のデバイスが接続されている場合はポートを指定します：
     ```bash
     esptool.py --chip esp32c3 --port /dev/ttyUSB0 --baud 921600 write_flash \
       0x1000 XenoOS.bootloader.bin \
       0x8000 XenoOS.partitions.bin \
       0x10000 XenoOS.bin
     ```
これらの手順を完了すると、システムがデバイスで実行されているはずです。

---

### Arduino IDE経由の標準インストール

システムをインストールしてセットアップするには、以下の手順に従ってください：

1. **前提条件**:
    - **Arduino IDE 2.x**がインストールされている必要があります。
    - Arduino IDEに**ESP32コアバージョン3.2.0以降**がインストールされていることを確認してください。
    - 以下のライブラリもインストールする必要があります：
      - `buildTime` バージョン 1.0.0
      - `ESPping` バージョン 1.0.5
      - `ESP32-targz` バージョン 1.2.6

2. **ハードウェアセットアップ**:
    - このシステムは**ESP32ファミリー**向けに設計されており、特に**ESP32-C3**向けに開発されています。
    - MicroSDカードモジュールには、**容量が4GB以下のSDカード**を使用してください。
    - **ESP32-C3**へのMicroSD接続は、公式ESP32-C3ピン配置に従った標準的なものです。

3. **セットアップ手順**:
    1. Gitを使用してこのリポジトリをローカルマシンにクローンします：
    ```bash
    git clone https://github.com/VLPLAY-Games/XenoOS.git
    cd XenoOS
    ```

    2. **Arduino IDE 2**で`XenoOS.ino`ファイルを開きます。
    
    3. Arduino IDEでターゲットデバイスとして**ESP32-C3 Dev Module**（またはお使いのボードに応じた別のESP32 Dev Moduleファミリー）を選択します。
    
    4. **ツール**タブで以下の設定を構成します：
    - **USB CDC On Boot**: "Enabled"
    - **CPU Frequency**: "160MHz (WiFi)"（デフォルト値）
    - **Core Debug Level**: "None"（デフォルト値）
    - **Erase All Flash Before Sketch Upload**: "Disabled"（デフォルト値）
    - **Flash Frequency**: "80MHz"（デフォルト値）
    - **Flash Mode**: "QIO"（デフォルト値）
    - **Flash Size**: "4MB (32Mb)"（デフォルト値）
    - **JTAG Adapter**: "Disabled"（デフォルト値）
    - **Partition Scheme**: "Minimal SPIFFS (1.9MB APP with OTA/190KB SPIFFS)"
    - **Upload Speed**: "921600"（デフォルト値）
    - **Zigbee Mode**: "Disabled"（デフォルト値）
    
    5. **アップロード**をクリックして、システムをESP32-C3にフラッシュします。

これらの手順を完了すると、システムがデバイスで実行されているはずです。

## 使用方法

インストール後、シリアルポート（速度115200）を介してPuttyでシステムと対話できます。端末またはコンソールに直接コマンドを入力します。利用可能なすべてのコマンドとその使用例については、上記の「コマンド一覧」を参照してください。

## 診断

システム診断を実行するには、以下のコマンドを使用します：

```bash
system diagnostic
system check
```

これにより、ESP32、SPIFFS、SDカード、EEPROMなどの様々なコンポーネントの健全性をチェックする一連のテストが実行されます。

## ライセンス
このプロジェクトはMITライセンスの下でライセンスされています - 詳細は[LICENSE](LICENSE)ファイルを参照してください。