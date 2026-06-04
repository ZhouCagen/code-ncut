from PIL import Image
image_path=Image.open('D:/1.jpg')
image_gray=image_path.convert('L')
image_path.show
image_gray.show
image_gray.save('D:/2.jpg')