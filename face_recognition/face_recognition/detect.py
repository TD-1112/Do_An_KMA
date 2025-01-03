import cv2
from skimage.feature import hog
from sklearn.decomposition import PCA
import pickle as pk
import joblib
import numpy as np

clf = joblib.load('c2_svm_classifier.pkl')
pca = pk.load(open("c1_PCA.pkl", 'rb'))
face_cascade = cv2.CascadeClassifier("haarcascade_frontalface_default.xml")

def detect_faces_and_recognize(frame):
    frame = cv2.resize(frame, (320, 240))
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    faces = face_cascade.detectMultiScale(gray, 1.1, 4)
    results = []

    for (x, y, w, h) in faces:
        face_roi = gray[y:y+h, x:x+w]
        resized = cv2.resize(face_roi, (64, 128))

        hog_features = hog(resized, orientations=9, pixels_per_cell=(8, 8),
                           cells_per_block=(2, 2), block_norm='L2-Hys', visualize=False)

        hog_features_pca = pca.transform([hog_features])

        probabilities = clf.predict_proba(hog_features_pca)
        confidence = np.max(probabilities)

        label = clf.predict(hog_features_pca)[0]

        if confidence < 0.8:
            results.append(('Unknown', (x, y, w, h)))
        else:
            if(label == 0):
                results.append(('Ha', (x, y, w, h)))
            else:
                results.append(('Thang', (x, y, w, h)))
    return results
