# handgesture.py
# Hai = kepalan tangan
# Saya Kirana = telunjuk
# Salam Kenal = dua jari (peace)
# Byee = lima jari terbuka

import cv2
import mediapipe as mp
import math
from collections import deque, Counter

mp_drawing = mp.solutions.drawing_utils
mp_hands = mp.solutions.hands

# Hitung jarak antar titik
def euclid(a, b):
    return math.hypot(a[0]-b[0], a[1]-b[1])

# Deteksi jari yang terbuka
def fingers_up(norm_lm):
    tips = [4, 8, 12, 16, 20]
    pip = [2, 6, 10, 14, 18]
    status = [False]*5

    # Thumb (horizontal)
    if abs(norm_lm[4][0] - norm_lm[3][0]) > 0.05:
        status[0] = True

    # Other fingers: tip.y < pip.y
    for i in range(1, 5):
        if norm_lm[tips[i]][1] < norm_lm[pip[i]][1] - 0.02:
            status[i] = True
    return status

# Identifikasi gesture
def recognize(norm_lm, px_lm):
    if not norm_lm or not px_lm:
        return "UNKNOWN"

    fup = fingers_up(norm_lm)

    # ✊ Fist
    if not any(fup):
        return "FIST"

    # ☝️ Index only
    if fup[1] and not any(fup[i] for i in [0,2,3,4]):
        return "INDEX_ONLY"

    # ✌️ Peace
    if fup[1] and fup[2] and not fup[3] and not fup[4]:
        return "PEACE"

    # 🖐️ Five open
    if all(fup):
        return "FIVE_OPEN"

    return "UNKNOWN"

def main():
    cap = cv2.VideoCapture(0)
    if not cap.isOpened():
        print("Cannot open webcam")
        return

    HISTORY = 8
    buffer = deque(maxlen=HISTORY)

    msg_map = {
        "FIST": "hai",
        "INDEX_ONLY": "saya kirana",
        "PEACE": "salam kenal",
        "FIVE_OPEN": "byee"
    }

    with mp_hands.Hands(
        max_num_hands=1,
        min_detection_confidence=0.6,
        min_tracking_confidence=0.6
    ) as hands:
        while True:
            ret, frame = cap.read()
            if not ret:
                break

            frame = cv2.flip(frame, 1)
            h, w, _ = frame.shape
            rgb = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
            results = hands.process(rgb)

            label = "UNKNOWN"

            if results.multi_hand_landmarks:
                for hand_landmarks in results.multi_hand_landmarks:
                    mp_drawing.draw_landmarks(frame, hand_landmarks, mp_hands.HAND_CONNECTIONS)

                    norm_lm = [(lm.x, lm.y) for lm in hand_landmarks.landmark]
                    px_lm = [(int(lm.x * w), int(lm.y * h)) for lm in hand_landmarks.landmark]

                    gesture = recognize(norm_lm, px_lm)
                    buffer.append(gesture)

                    label = Counter(buffer).most_common(1)[0][0]

                    text = msg_map.get(label, "")
                    if text:
                        cv2.putText(frame, text, (50, 100),
                                    cv2.FONT_HERSHEY_SIMPLEX, 1.5, (0, 255, 0), 3)

            cv2.putText(frame, "Fist: hai | Index: saya kirana | Peace: salam kenal | Open: byee",
                        (10, h - 20), cv2.FONT_HERSHEY_SIMPLEX, 0.55, (200, 200, 200), 1)
            cv2.putText(frame, "Press 'q' to quit", (10, 30),
                        cv2.FONT_HERSHEY_SIMPLEX, 0.7, (200, 200, 200), 2)

            cv2.imshow("Hand Gesture Recognition", frame)

            if cv2.waitKey(1) & 0xFF == ord('q'):
                break

    cap.release()
    cv2.destroyAllWindows()

if __name__ == "__main__":
    main()