import mss
import time
from PIL import Image
import io
import asyncio
from bleak import BleakClient

DEVICE_ADDRESS = "AA:BB:CC:DD:EE:FF"  # mac ADDRESS
CHARACTERISTIC_UUID = "0000xxxx-0000-1000-8000-00805f9b34fb"  

async def main():
    async with BleakClient(DEVICE_ADDRESS) as client:
        print("Connection succeed.")

        with mss.mss() as sct:
            monitor = sct.monitors[1]

            try:
                while True:
                    sct_img = sct.grab(monitor)
                    img = Image.frombytes('RGB', sct_img.size, sct_img.rgb)
                    img = img.resize((sct_img.width // 2, sct_img.height // 2))

                    buffer = io.BytesIO()
                    img.save(buffer, format='JPEG', quality=50)
                    img_bytes = buffer.getvalue()

                    # paquet size
                    CHUNK_SIZE = 20
                    chunks = [img_bytes[i:i + CHUNK_SIZE] for i in range(0, len(img_bytes), CHUNK_SIZE)]

                    for chunk in chunks:
                        await client.write_gatt_char(CHARACTERISTIC_UUID, chunk)
                        await asyncio.sleep(0.01)

                    await asyncio.sleep(0.1)

            except KeyboardInterrupt:
                print("User stopped the process.")

    print("Connection lost.")

asyncio.run(main())
