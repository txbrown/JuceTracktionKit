// Sources/JuceTracktionKit/TracktionEngine.swift
import CJuceTracktion

@available(macOS 14.0, iOS 17.0, *)
public final class TracktionEngine {
    let bridge: UnsafeMutablePointer<TracktionEngineBridge>
    
    public init() {
        let pointer = UnsafeMutablePointer<TracktionEngineBridge>.allocate(capacity: 1)
        bridge = pointer
        bridge.pointee.initialize()
    }
    
    deinit {
        bridge.pointee.shutdown()
    }
}
