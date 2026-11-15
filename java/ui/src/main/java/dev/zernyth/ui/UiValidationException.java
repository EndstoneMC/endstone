package dev.zernyth.ui;

import java.util.List;

/** Thrown when a UI document is not valid; {@link #problems()} lists every finding. */
public final class UiValidationException extends IllegalArgumentException {
    private static final long serialVersionUID = 1L;

    /** The findings; not serialized. */
    private final transient List<UiDiagnostic> problems;

    /**
     * Creates the exception.
     *
     * @param problems the findings, at least one
     */
    public UiValidationException(List<UiDiagnostic> problems) {
        super(problems.isEmpty() ? "Invalid UI document" : problems.getFirst().message()
                + (problems.size() > 1 ? " (and " + (problems.size() - 1) + " more)" : ""));
        this.problems = List.copyOf(problems);
    }

    /**
     * The findings.
     *
     * @return every problem found
     */
    public List<UiDiagnostic> problems() {
        return problems;
    }
}
