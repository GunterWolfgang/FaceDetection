#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main() {
    VideoCapture video("h.mp4");
    CascadeClassifier detectorRostros;
    Mat imagen;

    if (!detectorRostros.load("haarcascade_frontalface_default.xml")) {
        cerr << "Error al cargar el clasificador." << endl;
        return -1;
    }

    while (true) {
        video.read(imagen);
        if (imagen.empty()) break;

        vector<Rect> rostros;

        // Redimensionar la imagen para detección más rápida
        Mat imagenRedimensionada;
        resize(imagen, imagenRedimensionada, Size(), 0.4, 0.4); // Reduce al 40% del tamaño original

        // Detectar rostros en la imagen redimensionada
        detectorRostros.detectMultiScale(
            imagenRedimensionada, rostros, 1.1, 4, 0, Size(70, 70)); // Ajustar parámetros para mayor velocidad

        // Ajustar las coordenadas de los rostros detectados al tamaño original
        for (int i = 0; i < rostros.size(); i++) {
            rostros[i].x = static_cast<int>(rostros[i].x / 0.4);
            rostros[i].y = static_cast<int>(rostros[i].y / 0.4);
            rostros[i].width = static_cast<int>(rostros[i].width / 0.4);
            rostros[i].height = static_cast<int>(rostros[i].height / 0.4);
        }

        // Dibujar los rostros detectados y agregar nombres
        for (int i = 0; i < rostros.size(); i++) {
            rectangle(imagen, rostros[i].tl(), rostros[i].br(), Scalar(50, 50, 255), 20);

            // Agregar un nombre encima del cuadro
            string nombre = "Roxana " + to_string(i + 0); // Nombre dinámico basado en el índice
            putText(imagen, nombre, Point(rostros[i].x, rostros[i].y - 10), FONT_HERSHEY_SIMPLEX, 0.8, Scalar(500, 500, 500), 2);
        }

        // Mostrar el número de rostros detectados
        string texto = to_string(rostros.size()) + " Rostros Encontrados";
        int baseLine = 0;
        Size textSize = getTextSize(texto, FONT_HERSHEY_DUPLEX, 1, 1, &baseLine);

        // Ajustar el rectángulo al tamaño del texto
        rectangle(imagen, Point(0, 0), Point(textSize.width + 20, textSize.height + 20), Scalar(50, 50, 255), FILLED);
        putText(imagen, texto, Point(10, textSize.height + 10), FONT_HERSHEY_DUPLEX, 1, Scalar(255, 255, 255), 1);

        // Redimensionar la imagen para mostrarla en tamaño mediano
        Mat imagenMediana;
        resize(imagen, imagenMediana, Size(), 0.5, 0.5); // Reduce al 50% del tamaño original para mostrar

        imshow("Deteccion de Rostros", imagenMediana);

        // Reducir el tiempo de espera para mayor velocidad
        if (waitKey(1) == 27) break; // Tiempo de espera reducido a 1 ms
    }

    return 0;
}
