from PIL import Image
image_path=Image.open('D:/1.jpg')
image_resized=image_path.resize((300,300))
image_resized.show()
image_resized.save('D:/3.jpg')
