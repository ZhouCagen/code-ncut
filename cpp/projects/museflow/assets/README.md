`mock-demo.wav` 是 MuseFlow 自带的 8 秒演示音频，由 `scripts/generate_mock_audio.py` 通过正弦波和包络合成，不包含第三方录音或采样。

格式：22,050 Hz、16-bit、双声道 PCM WAV。它用于验证后台生成流程、播放、下载和版本保存，不代表 AI 模型的创作效果。所有 Mock 任务使用同一音频。

重新生成：`python3 scripts/generate_mock_audio.py`。正常构建直接使用已随项目保存的 WAV，不需要运行该脚本。
