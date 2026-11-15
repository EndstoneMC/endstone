package dev.zernyth.ui;

import java.util.Objects;

/**
 * One finding about a UI document.
 *
 * @param code stable rule code shared with UI Studio: {@code schema}, {@code duplicate_id},
 *     {@code unknown_screen}, {@code undeclared_action} or {@code missing_asset}
 * @param message human-readable explanation
 * @param path where the problem is, for example {@code screens[0].components[1]}
 */
public record UiDiagnostic(String code, String message, String path) {
    /**
     * Validates the parts.
     *
     * @param code rule code
     * @param message explanation
     * @param path location in the document
     */
    public UiDiagnostic {
        Objects.requireNonNull(code);
        Objects.requireNonNull(message);
        Objects.requireNonNull(path);
    }
}
