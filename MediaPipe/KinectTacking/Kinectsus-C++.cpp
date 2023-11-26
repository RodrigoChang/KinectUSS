#include <iostream>
#include <mediapipe/framework/port/opencv_highgui_inc.h>
#include <mediapipe/framework/port/opencv_imgproc_inc.h>
#include <mediapipe/framework/port/file_helpers.h>
#include <mediapipe/framework/formats/image_frame.h>
#include <mediapipe/framework/formats/image_frame_opencv.h>
#include <mediapipe/framework/calculator_framework.h>
#include <mediapipe/framework/calculator_graph.h>
#include <mediapipe/framework/calculators/tensorflow/tensorflow_lite_inference_calculator.pb.h>

constexpr char kInputStream[] = "input_video";
constexpr char kOutputStream[] = "output_video";

int main() {
    // Crear un grafo de cálculo de MediaPipe.
    mediapipe::CalculatorGraphConfig config =
        mediapipe::ParseTextProtoOrDie<mediapipe::CalculatorGraphConfig>(R"(
        input_stream: "input_video"
        output_stream: "output_video"
        node {
          calculator: "HandLandmarksCalculator"
          input_stream: "IMAGE:input_video"
          output_stream: "LANDMARKS:output_video"
        })");

    mediapipe::CalculatorGraph graph;
    MP_RETURN_IF_ERROR(graph.Initialize(config));

    // Abre el flujo de video.
    cv::VideoCapture capture(0);  // 0 para la cámara predeterminada. Puedes ajustar esto según tu configuración.

    if (!capture.isOpened()) {
        std::cerr << "Error al abrir la cámara." << std::endl;
        return -1;
    }

    // Inicia el bucle de procesamiento de video.
    while (true) {
        cv::Mat frame;
        capture >> frame;

        if (frame.empty()) {
            break;  // Fin del video.
        }

        // Convierte la imagen a un formato compatible con MediaPipe.
        cv::Mat input_frame;
        cv::cvtColor(frame, input_frame, cv::COLOR_BGR2RGB);
        auto input_frame_ptr = absl::make_unique<mediapipe::ImageFrame>(
            mediapipe::ImageFormat::SRGB, input_frame.cols, input_frame.rows, mediapipe::ImageFrame::kGlDefaultAlignmentBoundary);
        cv::Mat input_frame_mat = mediapipe::formats::MatView(input_frame_ptr.get());
        input_frame.copyTo(input_frame_mat);

        // Envía el fotograma al grafo de cálculo de MediaPipe.
        mediapipe::Packet packet = mediapipe::MakePacket<mediapipe::ImageFrame>(std::move(input_frame_ptr));
        MP_RETURN_IF_ERROR(graph.AddPacketToInputStream(kInputStream, packet));

        // Obtén el resultado del grafo de cálculo de MediaPipe.
        mediapipe::Packet result_packet;
        MP_RETURN_IF_ERROR(graph.GetNextPacket(&result_packet));
        const auto& output_frame = result_packet.Get<mediapipe::ImageFrame>();

        // Convierte el resultado a formato OpenCV.
        cv::Mat output_mat = mediapipe::formats::MatView(&output_frame);

        // Muestra el resultado.
        cv::imshow("MediaPipe Hand Landmarks", output_mat);

        // Maneja eventos de teclado.
        char key = cv::waitKey(1);
        if (key == 27) {  // 27 es el código ASCII para la tecla Esc.
            break;
        }
    }

    // Cierra la captura de video y destruye las ventanas de OpenCV.
    capture.release();
    cv::destroyAllWindows();

    return 0;
}
