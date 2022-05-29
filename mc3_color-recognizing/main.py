import cv2
import numpy as np


def gstreamer_pipeline(
    capture_width=1280,
    capture_height=720,
    display_width=1280,
    display_height=720,
    framerate=30,
    flip_method=0,
):
    return (
        "nvarguscamerasrc ! "
        "video/x-raw(memory:NVMM), "
        "width=(int)%d, height=(int)%d, "
        "format=(string)NV12, framerate=(fraction)%d/1 ! "
        "nvvidconv flip-method=%d ! "
        "video/x-raw, width=(int)%d, height=(int)%d, format=(string)BGRx ! "
        "videoconvert ! "
        "video/x-raw, format=(string)BGR ! appsink max-buffers=1 drop=true"
        % (
            capture_width,
            capture_height,
            framerate,
            flip_method,
            display_width,
            display_height,
        )
    )



COLORS = [
    'Mid Red', 'Warm Red', 'Orange', 'Warm Yellow',
    'Mid Yellow', 'Cool Yellow', 'Yellow Green', 'Warm Green',
    'Mid Green', 'Cool Green', 'Green Cyan', 'Warm Cyan',
    'Mid Cyan', 'Cool Cyan', 'Blue Cyan', 'Cool Blue',
    'Mid Blue', 'Warm Blue', 'Violet', 'Cool Magenta',
    'Mid Magenta', 'Warm Magenta', 'Red Magenta', 'Cool Red',
]


N = len(COLORS)
SENSITIVITY = 180 / N
THRESHOLD = 0.6
RECT_SIZE = 100
STEP = RECT_SIZE // 2


def process(frame, start_point, end_point):
    color = (255, 255, 255)
    thickness = 2
    rect = cv2.rectangle(frame, start_point, end_point, color, thickness)
    rect_frame = frame[start_point[1]:end_point[1], start_point[0]:end_point[0]]
    hsv_frame = cv2.cvtColor(rect_frame, cv2.COLOR_BGR2HSV)

    color_text = ':|'

    h, s, v = cv2.split(hsv_frame)
    h = np.floor_divide(h, SENSITIVITY).astype(int)

    uniques, counts = np.unique(h, return_counts=True)
    for i, count in enumerate(counts):
        if count >= RECT_SIZE * RECT_SIZE * THRESHOLD:
            color_text = COLORS[uniques[i]]
            break

    font = cv2.FONT_HERSHEY_SIMPLEX
    font_scale = 0.7
    cv2.putText(rect, color_text, (10, 50), font, font_scale, color, thickness, cv2.LINE_AA)


def main():
    print('Press A, D, W, S to change position\n')
    print('Press 4 to Quit the Application\n')

    cap = cv2.VideoCapture(gstreamer_pipeline(flip_method=4), cv2.CAP_GSTREAMER)
    dx, dy = 0, 0

    while cap.isOpened():
        ret, frame = cap.read()

        if not ret:
            continue

        frame = cv2.flip(frame, 180)
        width, height = int(cap.get(cv2.CAP_PROP_FRAME_WIDTH)), int(cap.get(cv2.CAP_PROP_FRAME_HEIGHT))

        x1 = max(min(width // 2 - RECT_SIZE // 2 + dx, width - RECT_SIZE), 0)
        y1 = max(min(height // 2 - RECT_SIZE // 2 + dy, height - RECT_SIZE), 0)
        process(frame, (x1, y1), (x1 + RECT_SIZE, y1 + RECT_SIZE))

        cv2.imshow('Cam', frame)

        k = chr(cv2.waitKey(1) & 0xFF).lower()
        if k == '4':
            print('Good Bye')
            break

        if k == 'a':
            dx -= STEP
        elif k == 'd':
            dx += STEP
        elif k == 'w':
            dy -= STEP
        elif k == 's':
            dy += STEP

    cap.release()
    cv2.destroyAllWindows()


if __name__ == '__main__':
    main()
